## mnhistory.rb: internal history list for a body of all the computed
##               derivatives of the body's position: pos, vel, acc, etc.
##
## Piet Hut, 2004/04
##
## A history object h is an array containing particle state information s,
## for successive times t, as follows:
##
##   h = [ [t0, s0], [t1, s1], . . . , [ti, si], . . . ]
##
##   si = [ pos, vel, acc, . . . ]
##
## expressed explicitly in components, the h values are:
##
##   h[0] = oldest {time, state} pair
##   h[0][0] = oldest time t0
##   h[0][1] = state at t0: an array of {0th, 1th, ...} derivatives of position
##   h[0][1][0] = value of position vector at t0
##   h[0][1][0][k] = value of kth component of position vector at t0
##   h[0][1][1] = value of velocity vector at t0
##   h[0][1][2] = value of acceleration vector at t0
##   h[1] = second {time, state} pair
##   h[1][0] = second time t1
##   h[1][1] = state at t1
##   h[1][1][0] = value of position vector at t1
##   etc.
## 
## Upon creation, a history list h contains at least one value, the oldest 
## time t0.  In addition, a state array is created of zero length, ready to
## be extended automatically when values are assigned to pos, vel, acc, etc.
## The initial structure is thus:
## 
##   h = [[0.0, []]]
## 
## After creation, h can only grow.  Before doing anything dynamic, one has
## to provide the particle with an initial position and velocity.  At any
## given time after that, h can take on the following four forms:
##
## -- predicting: 
##    the particle has a `latest sales date', h.last[0], up to which time
##    it can predict its state, even though its actual state h.last[1] has
##    not yet been computed: h.last[1] = [], an empty array.  Note that its
##    penultimate state includes at least an acceleration h[h.size-2][1][2]
##    and possibly higher derivatives, such as jerk h[h.size-2][1][3], etc.
##
## -- arrived: 
##    the particle position and velocity have been updated to its latest time
##    h.last[0], and so h has no future `shelf life': it can only postdict its
##    state, since it has not yet received values for acceleration, jerk, etc.:
##    h.last[1] = [pos, vel].
## 
## -- incomplete: 
##    while moving between the two stable states `predicting' and `arrived',
##    the particle may call other particles that in turn may call the particle
##    itself, and find it in an incomplete state; I'm not yet sure whether and
##    if so, how often and how this may happend, but it would be prudent to
##    allow for this possibility in designing the History class.
## 
## After reading in the initial position and velocity, h takes on the form of
## an `arrived' structure.  After calculating the higher derivates and the
## time step deduced from that, h takes the `predicted' form.  The particle
## can then use the prescribed integration algorithm to step forwards, to reach
## again at an `arrived' structure, and so on.
##
## Note: there is one extra twist: we need to give all particles an acc before
##       any body can determine a time step (which asks other particles for
##       their acc); this is done with an "acc_initializing" flag in Body.new

require "mnvector.rb"

class History
  def initialize(time = 0)
    @history = Array.new
    @history[0] = Array.new
    @history[0][0] = time.to_f             # first time
    @history[0][1] = Array.new             # array of derivatives of position
  end
  def set_first_time(t)
    @history[0][0] = t
  end
  def extend(t, history_depth)
    new_index = @history.size
    @history[new_index] = Array.new
    @history[new_index][0] = t
    @history[new_index][1] = Array.new
    while @history.size > history_depth    # prune
      @history[0] = nil
      @history.compact!
    end
  end
  def set_last_rndot(n, rndot)             # rndot = d^n r / dt^n
    @history.last[1][n] = rndot
  end
  def set_penultimate_rndot(n, rndot)
    @history[@history.size-2][1][n] = rndot
  end
  def arrived?
    @history.last[1][0] and @history.last[1][1]
  end
  def predicting?
    (not @history.last[1][0]) and @history[@history.size-2][1][2]
  end
  def ndim
    @history[0][1][0].size
  end
  def earliest
    if @history[1] and @history[1][0]
      2*@history[0][0] - @history[1][0]    # allow backwards extrapolation
    else
      @history[0][0]
    end
  end
  def latest
    if @history.last[0]
      @history.last[0]
    else
      @history[(@history.size)-2][0]
    end
  end
  def before_latest
    if @history.last[0]
      @history[(@history.size)-2][0]
    else
      @history[(@history.size)-3][0]
    end
  end
  def last_index
    (@history.size) - 1
  end
  def index_before(t)
    if t < earliest
      nil
    elsif t < @history[0][0]
      -1                                   # allow backwards extrapolation
    else
      i = 1
      while @history[i] and @history[i][0] and @history[i][0] <= t
        i += 1
      end
      i - 1
    end
  end
  def index_after(t)
    if t > latest
      nil
    else
      if @history[(@history.size) -1][0]
        i = (@history.size) - 2
      else
        i = (@history.size) - 3
      end
      while i >= 0 and @history[i][0] >= t
        i -= 1
      end
      i + 1
    end
  end
  def extrapolate(n, i, dt)  # for now, uses only derivatives at starting point
    if not @history[i][1][n]
      nil
    else
      dn = 0
      while @history[i][1][n+dn+1]
        dn += 1
      end
      # evaluate Taylor series, starting with the highest derivative
      # example: r + v.dt + a.dt^2/2 + j.dt^3/6 =
      #          r + (v + (a + (j + 0) * dt / 3) * dt /2) * dt / 1
      increment = Vector.new(ndim, 0.0)
      while (dn > 0)
        increment += @history[i][1][n+dn]
        increment *= dt/dn
        dn -= 1
      end
      @history[i][1][n] + increment
    end
  end
  def improved_extrapolate(n, i, dt)     # extend Taylor series by one term.
    if i == 0                            #   if no past information:
      increment = Vector.new(ndim, 0.0)  #     return zero
    else                                 #   if there is past information:
      dn = 0                             #     use previous time to estimate
      while @history[i][1][n+dn+1]       #     one higher order Taylor term
        dn += 1                          #     (first value of increment)
      end                                #
      increment = (@history[i][1][n+dn] - @history[i-1][1][n+dn]) *
                  (1.0/ (@history[i][0] - @history[i-1][0]) )
      dn += 1
      while (dn > 0)
        increment *= dt/dn               # computer Taylor coefficient
        dn -= 1
      end
    end
    extrapolate(n, i, dt) + increment
  end
  def rndot(n,t)
    if t < earliest or t > latest
      nil
    else
      i = index_before(t)
      j = index_after(t)
      if i == j
        if @history[i][1][n]
          @history[i][1][n]
        elsif @history[i-1][1][n]
#          extrapolate(n, i-1, t - @history[i-1][0])
          improved_extrapolate(n, i-1, t - @history[i-1][0])
        else
          nil
        end
      else
        i = 0 if i == -1                        # allow backwards extrapolation
        extrapolate(n, i, t - @history[i][0])
      end
    end
  end
  def pp                               # pretty print
    print "  ["
    init_flag = true
    @history.each do |x|
      if init_flag
        print "["
        init_flag = false
      else
        print "   ["
      end
      p x[0]
      x[1].each do |y|
        print "       "
        p y
      end
      print "   ]\n"
    end
    print "  ]\n"
  end
end
