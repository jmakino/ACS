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
## After creation, h can only grow.  At any given time, h can exist in various
## degrees of completion.  
one of two legal formats: a `potential' format and an `actual' format.
## 
## In the potential format, the last h entry contains a time, but no state,
## while the penultimate h entry contains a time and a state with at least
## an acceleration (and possibly higher derivatives, such as a jerk, etc.).
## 
## In the actual format, the last h entry contains a time, and 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 
## 

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
  def extend(t)
    new_index = @history.size
    @history[new_index] = Array.new
    @history[new_index][0] = t
    @history[new_index][1] = Array.new
  end
  def set_last_rndot(n, rndot)             # rndot = d^n r / dt^n
    @history.last[1][n] = rndot
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
      increment = 0
      while (dn > 0)
        increment += @history[i][1][n+dn]
        increment *= dt/dn
        dn -= 1
      end
      @history[i][1][n] + increment
    end
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
          extrapolate(n, i-1, t - @history[i-1][0])
        else
          nil
        end
      else
        i = 0 if i == -1                        # allow backwards extrapolation
        extrapolate(n, i, t - @history[i][0])
      end
    end
  end
end
