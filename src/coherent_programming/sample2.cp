<tex>
\title{The Art of Computational Science\\
~\\
Sample texts for ACSDOC.RB\\
~\\
 Sample 2: Graphics
}
\author{Piet Hut and Jun Makino}
\maketitle
%\newpage
%\tableofcontents
</tex>

= Coherent Programming

This is the text for the first heading

== markups

Here I give samples for basic  markups

=== word mark


Fonts: *bold*, +typewriter+, and _italic_

The things should noe be interpreted: a_to_f_d.

More experiments on fonts: <em>italic phrase</em>, <tt>typewriter
words</tt>, and <b>boldface text</b>. How is this transformed?

Now here is <tex>latex inline</tex>

Here is #include, {text in curly blackets} etc.

*  test item 1
  * nested item
    * strange item
*  test item 2

Hmmmm...

  def xxx
  end

Hmmmm ....    

1. Numbered item
2. Numbered item

New paragraph

  def xxx
     difficult nesting
        more difficult nesting
     one nest less
 * continued varbatim
Back to normal

listing stuff

   simple listing
   simple listing continued

The above should show up as preformatted

      Simple list
   impossible next line   

---
 :include:sample.rb
---

This is sample image link: link:sample.gif

How is it translated?

=== complex tex markup sample

Here <tex>$M_j$</tex> and <tex>${\bf r}_j$</tex> are the mass and
position vector of particle <tex>$j$</tex>, and <tex>$G$</tex> is the
gravitational constant.  To bring out the inverse square nature of
gravity, we can define
<tex>${\bf r}_{ji} = {\bf r}_j - {\bf r}_i$</tex>, with
<tex>$r_{ji} = |{\bf r}_{ji}|$</tex>, and unit vector
<tex>$\hat {\bf r}_{ji} = {\bf r}_{ji} / r_{ji}$</tex>.  The
gravitational acceleration on particle <tex>$i$</tex> then becomes:

<tex>
$$
{\bf a}_i = G \sum_{j=1 \atop j \neq i}^N
\frac{M_j}{r_{ji}^2} \,\hat{\bf r}_{ji}
$$
</tex>

The following is the same as the above formula, but using : equation : 

:equation:
{\bf a}_i = G \sum_{j=1 \atop j \neq i}^N
\frac{M_j}{r_{ji}^2} \,\hat{\bf r}_{ji}
\label{newton}

The following is an equation array

:eqnarray:
\label{samplearray}
a &=& b\nonumber\\
b &=& c

Equation (ref(newton)) is the equation of motion for <i>N</i>-body
problem. Equations (ref(samplearray)) just show two equations. Note
that curently equation (or equnarray) can have only one number. So
with eqnarray, all but one fomula must be used as "nonumber", as in
the above example.

The next example is an figure:

:figure: sample.gif 5cm acslogo
The ACS logo

There may be more explanations needed...


