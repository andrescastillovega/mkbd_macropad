this proyect is a split macropad (left macropad as master that handles 
communication with the pc, and the right connects wirelessly to the master. 
any key stroke is sent from the master to the pc regardless of the stroke 
happened on the right or left side). The firmware is done with ZMK which has 
Zephyr bellow it.

both sides work with a niceNano v2. The left side has a screen.

each side has 5 rows and 6 columns.

Common Row pins 4,5,6,7,8

Left Columns pins 21,20,19,18,15,14.

Right Columns pins 14,15,18,19,20,21.
