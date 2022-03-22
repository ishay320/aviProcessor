# Aruco Calibration

1. print aruco board
1. `make` the project
1. run calibration with arguments:<br>
    -w=Number of squares in X direction<br>
    -h=Number of squares in Y direction<br>
    -l=Marker side length (in meters)<br>
    -s=Separation between two consecutive markers in the grid (in meters)<br>
    -d=dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,
                    DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7, 
                    DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,
                    DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL = 16<br>
    -v=Input from video file, if ommited, input comes from camera<br>
    -ci=Camera id if input doesnt come from video (-v) - default=0<br>
    -dp=File of marker detector parameters<br>
    -rs=Apply refind strategy - default=false<br>
    -zt=Assume zero tangential distortion - default=false<br>
    -a=Fix aspect ratio (fx/fy) to this value<br>
    -pc=Fix the principal point at the center - default=false<br>
    -waitkey=Time in milliseconds to wait for key press - default=10<br>
    and the last: Output file with calibrated camera parameters<br>

    for example: `./calibration -d=8 -h=7 -l=0.033 -s=0.003 -w=5 calibration.json`
1. press `c` to capture and `ESC` to finish
1. use the calibration :)

replace line 215 to fit the camera 


## arucoTest:
is here to test if all went well, replace camera capture to fit your camera.
`make arucoTest`