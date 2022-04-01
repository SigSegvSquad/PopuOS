# PopuOS

### Overview
You guys are in charge of this

### Sample Programs
Put these in Input.txt and run

```
$AMJ000100030001
GD10
PD10
H
$DTA
Hello
$END0001
```

Prints Hello
```
$AMJ000200030001
GD10
LR12
SR15
PD10
H
$DTA
I LIKE THIS PEN OF
$END0002
```
Should Print "I LIKE THIS PEN OF HIS"
```
$AMJ000300120001
GD20
PD20
GD30
PD30
GD40
GD50
LR20
CR30
BT11
PD50
H
PD40
H
$DTA
VIIT
VIIT
IS SAME
IS NOT SAME
$END0003
```
Compares strings given at line 38 and 39 (in this case VIIT and VIIT)

Omkar Prabhune - C71
No Error
$AMJ000000050001
GD10PD10
$DTA
Sample
$END

Out of Data
$AMJ000100020001
GD10PD10
$DTA
$END

Time Limit Exceeded
$AMJ000200020001
GD10PD10
$DTA
Sample
$END

Line Limit Exceeded
$AMJ000300050000
GD10PD10
$DTA
Sample
$END

Opcode Error
$AMJ000400050001
FD10PD10
$DTA
Sample
$END

Operand Error
$AMJ000600050001
GDxxPD10
$DTA
Sample
$END

Page Fault
$AMJ000700050001
GD10PD20
$DTA
Sample
$END