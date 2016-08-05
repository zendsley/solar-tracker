# Zack Endsley
# ECE492
# 2016_08_03
# This cherrypy server is designed to be the UI component for the solar tracker project

import cherrypy
import serial
import time, os, glob

# Constant representing serial port of Arduino Uno device.
# Locate port of device and change as needed
PORT = '/dev/tty.usbmodem1451'

# Define serial port connection & baud rate.  More parameters are availble, but left as default
ser = serial.Serial(
    port=PORT,
    baudrate=9600
    )

# Saves raw input from serial stream to list; each list member is length of the specified byte size
# Assumes serial_feed is already .open() thus live
def read_serial(live_serial_feed, bytes=5):
    serial_data = []
    # while live_serial_feed.out_waiting > 0 :
    #     sleep(1)
    while live_serial_feed.inWaiting() > 0 :
    #live_serial_feed.flush()
    #for i in range(bytes):
        serial_data.append(live_serial_feed.readline())
    return serial_data

# Writes given output to a live serial feed in hex bytes from given data list
# This was originally thought to be a more complicated process, so this function was created;
# Left for legacy reasons
def write_serial(live_serial_feed, data):
    live_serial_feed.write(data)
    live_serial_feed.flush()
    return

def strip_serial(serial_data):
    clean_data = []
    print 'lum, x, y'
    for i, val in enumerate(serial_data):
        clean_data.append(val.strip(';\r\n'))
        print clean_data[i]
    return clean_data

# Objects for each data point are a bit overkill...
# class LightPoint:
#     def __init__(self, light_intensity, x_val, y_val):
#         self.light_intensity = light_intensity
#         self.x_val = x_val
#         self.y_val = y_val
#     def __repr__(self):
#         return repr((self.light_intensity,self.x_val,self.y_val))



# CherryPy portion of code; mostly html defining pages and function calls to above functions where needed
class StringGenerator(object):
    @cherrypy.expose
    def index(self):

        ### Index page format ###
        header = """ <html>
            <head>
            <link href="/static/css/style.css" rel="stylesheet">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>Solar Tracker</title>
            </head>
            <body>"""
        intro_line = """<h1>Solar Tracker</h1> <br> Welcome to Solar Tracker!
        Specify degrees of precision (out of 180):<br>"""

        input_box = """<form method="get" action="scan">
                    <input type="number" name="precision" min="1" max="180" value="10">
                    <br><br>
                    <button type="submit" style="font-size:10px">Submit</button>
                </form>"""

        footer = "</body></html>"
        output = " <br>\n".join([header, intro_line, input_box, footer])
        return output

    @cherrypy.expose
    def scan(self, select=1, scan_range=180, precision=20, x=90, y=90):
        #data_points = sweep_sky(precision)
        #max_light_point = find_max_point(data_points)
       
        # Test data for page creation purposes; remove once serial is running
        max_light_point = [450,20,60]

        # Create list and append values to output over serial
        scan_instructions = []
        scan_instructions.append(int(select))
        scan_instructions.append(int(scan_range))
        scan_instructions.append(int(precision))
        scan_instructions.append(int(x))
        scan_instructions.append(int(y))

        # Print output to commandline for debugging/verification purposes
        # Might as well leave this to enable data verification on the backend
        print 
        print scan_instructions
        print
        print 'select=', select
        print 'scan_range=', scan_range
        print 'precision=', precision
        print 'x=', x
        print 'y=', y
        print
        write_serial(ser,scan_instructions)
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        time.sleep(2)
        # Read serial data from Arduino Uno
        scan_results = []
        scan_results = read_serial(ser,3)
        print 'scan results:'
        print scan_results
        print
        clean_data = []
        clean_data = strip_serial(scan_results)
        print
        print 'clean results'
        print clean_data
        print


        ### Scan page format ###
        header = """ <html>
            <head>
            <link href="/static/css/style.css" rel="stylesheet">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
            table {
                font-family: arial, sans-serif;
                border-collapse: collapse;
                width: 50%;
            }

            td,th {
                border: 1px solid #dddddd;
                text-align: left;
                padding: 8px;
            }

            tr:nth-child(even) {
                background-color: #ffff33;
            }
            </style>
            <title>Solar Tracker v</title>  
            </head>
            <body>"""
        intro_line = "<h2>Results:</h2>"

        results_table = """
            <table>
                <tr>
                    <th>Light Intensity</th>
                    <th>Servo Arm Position</th>
                    <th>Servo Base Position</th>
                </tr>
                <tr>
                    <th>%s</th>
                    <th>%s</th>
                    <th>%s</th>
                </tr>
            </table>
            """ % (max_light_point[0], max_light_point[1], max_light_point[2])


        input_box = """<form method="get" action="index">
                    <br>
                    <button type="submit" style="font-size:10px">Scan Again?</button>
                </form>"""

        footer = "</body></html>"
        output = " <br>\n".join([header, intro_line, results_table, input_box, footer])
        return output

if __name__ == "__main__" :
    conf = {'server.socket_host': '0.0.0.0'}
    cherrypy.config.update(conf)
    cherrypy.quickstart(StringGenerator(),config={'/':conf})
