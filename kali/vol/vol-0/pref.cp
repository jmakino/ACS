:nosectionnumber:
	
= Preface

== A New Book Series

This book is the first in a series of books titled <i>The Art of
Computational Science.</i> The aim of the series is to provide
real-world guides to designing, developing, using, and extending
computational laboratories.  In many areas of science, computer
simulations of complex physical systems cannot be performed with
off-the-shelf software packages.  Instead, computational scientists
have to design and build their own software environment, just as
experimental scientists have to design and build their own
laboratories, before being able to use them.

The first few volumes in this series will focus on one particular
topic, the <i>gravitational N-body problem</i>, the oldest unsolved
problem in mathematical physics, and an area of active research in
astrophysics, with applications ranging from star formation to
collisions between normal stars, neutron stars and black holes, with
implication for the growth of structure in the universe on all scales,
from that of clusters of galaxies down to the formation of planets and
their satellites.

One reason to choose this topic is to provide an example of a
computational lab where the underlying physics is very simple, just
Newton's classical law of gravity.  The complexity lies exclusively in
the emergent properties that show up in the paradoxical thermodynamics
of self-gravitating systems, where negative specific heat and other
conundrums drive each system far out of equilibrium in ways that are
theoretically still ill understood, yet can be modeled accurately in
computational labs.

Given the simplicity of at least the underlying physics, we hope
that the venerable N-body problem can function as a paradigm for the
exploration of new principles and methods for a novel generation of
computational laboratories.  With this idea in mind, we are trying to
present these first few books in sufficient detail for researchers
from other areas of natural science to be able to follow and
appreciate the approach to virtual lab building presented here.

Future volumes in this series are expected to cover wider and wider
areas of science.  Most likely, we will move from stellar dynamics,
associated with the gravitational N-body problem, first to stellar
evolution and stellar hydrodynamics, followed by other areas of
simulations in astrophysics and physics in general.  But before too
long, we hope to attract chemists and biologists, and well as
scientists from other areas.  If they find our approach useful, and
like to try it out in their own field of expertise, we will welcome
them to join this series, and to provide additional volumes discussing
computational labs for their specialty.

Finally, we will also welcome more educational volumes to appear in
this series.  The current authors already have some detailed plans in
that regard, aimed at late high school to early undergraduate levels.
In those volumes we would like to introduce the basic notions of
physical forces and resultant motions, the associated mathematical
description through differential equations, and numerical techniques
for finding general solutions to those equations.  Here, too, we
welcome others to join us with similar educational approaches, applied
to other topics.

== A Narrative in Dialogue Form

The format we have chosen for our book series, at least for the first
several volumes, is sufficiently unusual that it deserves some
clarification.  Most scientific text books summarize the state of the
art of a field `after the dust has settled.'  Students are presented
with a clean path of logic that bears little resemblance to the
historical process of trial and error that underlies any scientific
discovery, large or small.  As a result, a graduate student facing his
or her first major piece of independent research is in for a big shock,
suddenly realizing how hard it is to derive anything new with little
or no guidance.

In practice, the blow is softened in many cases by conversations
with a range of helpers, from fellow students, somewhat more senior
postdocs, as well as thesis advisers and other faculty members.
However, not everyone is so lucky as to find sufficiently helpful
advice that way, and in a world of increasing specialization, the
advice of even the most well intended helpers may not suffice for
the problem at hand.  This is especially true in a rapidly evolving
area such as computational science, where the most basic tools, from
languages to packages to the very approach to programming, have
changed so much even in the last ten years.

Browsing in a book store is not likely to be very helpful either.
While there are many books that discuss algorithms and programming
languages, those are necessary but far from sufficient ingredients for
building a computational lab.  What is needed is a practical form of
know-how, of how to approach the whole process of lab building, from
developing an overarching vision to connecting it to nitty-gritty
details.  To present this form of know-how, via case studies, is the
central aim of our book series.

Given this wide ranging challenge, a traditional approach of just
summarizing our basic points would most likely be insufficient to let
students apply our method to their own specific problem case.
Instead, our approach is based on giving a hands-on feel for the many
pitfalls and diversions on the road to the design and development of
large and complex software systems.  And the only real way to get such
direct experience is to make mistakes, many of them, and then to learn
from them, systematically.

Even the best researchers spent most of their time making mistakes; it
is the fuel for the engine of exploration.  Our aim in this series is
to share this approach with our readers, providing them with a sample
of the types of mistakes we have made in the the quarter century that
we have been working in computational science.  The best way to share
this atmosphere of trial and error, we think, is through a dialogue
between researchers who resemble ourselves, in their way of
formulating plans, noticing problems, adjusting their aims, finding
ways around stumbling blocks, in short, dealing with the joys and
frustrations of every-day research.

We could of course have written a fictional dialogue between the two
of us, distilling our experience of working together for much of our
professional life: since 1988 we have been coauthors on some fifty
scientific publications, and together we have organized two symposia
sponsored by the International Astronomical Union and edited the
corresponding proceedings, as well as a number of workshops.  However,
such an literal approach would have been too narrow.  We decided it
would be much more fun to use composite characters, mixing in some of
the styles and personalities of the few dozen colleagues we have
interacted with intensively over the years.

== ACS versions

We use the name <i>The Art of Computational Science</i> not only for
our book series, but more generally for the software environment for
which the books provide the narrative.  The environment includes the
collection of computer codes discussed in the books, together with the
infrastructure to make it all work together seemlessly.  This implies
extensive comments provided in the codes themselves, as well as manual
pages and detailed lab notes made by us while we developed the whole
package, code, book text, and everything else.  These lab notes, while
rough and really meant for our own use, provide an extra level of
background information, which may be useful in addition to the more
polished narrative provided through the dialogues.

Our plan is to make successive stable versions of this software
environment available, starting with ACS 0.1, which contains a small
but self-sufficient core of simple N-body programs and accompanying
documentation and narrative.  These versions can be freely downloaded
from our web site "http://www.ArtCompSci.org".  They include all
completed volumes in our book series.  Text, code, and everything else
is presented as open source software under the conditions of the MIT
license:  

    Copyright (c) 2004 -- present, Piet Hut & Jun Makino

    Permission is hereby granted, free of charge, to any
    person obtaining a copy of this software and associated
    documentation files (the "Software"), to deal in the
    Software without restriction, including without
    limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice
    shall be included in all copies or substantial portions
    of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
    TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
    SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
    IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

#In addition, we plan to make these volumes available in print, with
#the xxxx company.  We hope, of course, that our software environment
#will be sufficiently attractive that serious users will want to
#purchase printed copies of these books for their extra convenience; as
#a side effect, this may convince book publishers that there really is
#a market for `open source' book publishing.

== A Historical Note

In preparing for our project, we wrote a complete manuscript, titled
<i>Moving Stars Around</i>, also in dialogue form, but aimed more at
beginning students who may not yet have much familiarity with computer
programming and the use of numerical methods.  We do not plan to
publish that manuscript, which we put up on our web site in the Fall
of 2003, in its current form.  Instead, we intend to recycle much of
the text in one or more volumes in our current series.
However, until we find the time to do so, we will keep <i>Moving Stars
Around</i> around on our web site, as introductory material.

== Acknowledgments

Besides thanking our home institutes, the Institute for Advanced Study
in Princeton and the University of Tokyo, we want to convey our
special gratitude to the Yukawa Institute of Theoretical Physics in
Kyoto, where we have produced a substantial part of our ACS material,
including its basic infrastructure, during extended visits made
possible by the kind invitations to both of us by Professor Masao
Ninomiya.  In addition, we thank the Observatory of Strasbourg, where
the kind invitation of Professor Christian Boily allowed us to make a
rapid beginning with the current volume.

We thank Douglas Heggie, xxx, and xxx for their comments on the
manuscript.

Piet Hut and Jun Makino
