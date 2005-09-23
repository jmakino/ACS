printer fig1.ps/vcps
square
viewport 0.2 1.1 0.2 1.1
data logtest5a3
xcol 1
ycol 3
limit 0 2000 -1e-4 1e-3
expand 1.4
lw 2
box
conn
data logtest5a3nb
xcol 1
ycol 3
conn
limit 0 1 0 1
box 1 1
reloc 0.45 -0.15
expand 2
label T
reloc -0.25 0.6
label \\gDE
expand 1.3
reloc 0.6 0.3
label non-block
reloc 0.5 0.6
angle 0
label block
pgend
psfix

printer fig2.ps/vcps
square
viewport 0.2 1.1 0.2 1.1
data logtest4a0
xcol 1
ycol 3
limit 0 2000 -1e-4 1e-3
expand 1.4
lw 2
box BCNTSV BCNTSV
conn
data logtest5a3
xcol 1
ycol 3
conn
limit 0 1 0 1
box 1 1
reloc 0.45 -0.15
expand 2
label T
reloc -0.25 0.6
label \\gDE
expand 1.3
reloc 0.6 0.26
label corrected block
reloc 0.5 0.6
angle 0
label block
pgend
psfix
