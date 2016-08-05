# Zack Endsley
# ECE492
# 2016_08_03
# This cherrypy server is designed to be the UI component for the solar tracker project

import cherrypy
import serial
import time, os, glob
import numpy as np

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
        time.sleep(0.12)
    return serial_data

# Writes given output to a live serial feed in hex bytes from given data list
# This was originally thought to be a more complicated process, so this function was created;
# Left for legacy reasons
def write_serial(live_serial_feed, data):
    live_serial_feed.flushInput()
    live_serial_feed.flushOutput()
    live_serial_feed.write(data)
    live_serial_feed.flush()
    return

def strip_serial(serial_data):
    clean_data = []
    print 'lum, x, y'
    for i, val in enumerate(serial_data):
        clean_data.append(val.strip('\n'))
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

# Originally used lists to populate table; see numpy array for better method of number storage
def create_list(list):
    light_points = []
    for i in list:
        light_points.append(i.split(','))
    return light_points

# Used to create a matrix of light_intensity, x, and y values
def create_numpy_array(list):
        numpy_array = np.array([[float(j) for j in i.split(',')] for i in list])
        # numpy_array = np.array((float(j) for j in i.split(',')) for i in list)
        # print numpy_array
        return numpy_array

def find_max_light(array):
    max_val = 0
    max_val_index = 0
    for i, val in enumerate(array):
        if val[0] > max_val:
            max_val = val[0]
            max_val_index = i
    return max_val, max_val_index


# CherryPy portion of code; mostly html defining pages and function calls to above functions where needed
class StringGenerator(object):
    @cherrypy.expose
    def index(self):

        ### Index page format ###
        header = """ <html>
            <head>
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
        
        time.sleep(2)
        # Read serial data from Arduino Uno
        scan_results = []
        scan_results = read_serial(ser,3)
        ser.flushInput()
        ser.flushOutput()

        print 'scan results:'
        print scan_results
        print
        clean_data = []
        clean_data = strip_serial(scan_results)
        # print
        # print 'clean results'
        # print clean_data
        # print
        # print 'list'
        # print
        # data_list = create_list(clean_data)
        # print data_list
        print
        print 'numpy'
        print
        numpy_array = create_numpy_array(clean_data)
        print numpy_array
        print
        print 'max light value'
        print
        max_light_value, max_light_value_index = find_max_light(numpy_array)
        print max_light_value
        print numpy_array[max_light_value_index]
        print
        lock_on_instructions = []
        lock_on_instructions.append(int(select))
        lock_on_instructions.append(int(1))
        lock_on_instructions.append(int(1))
        lock_on_instructions.append(int(numpy_array[max_light_value_index][1]))
        lock_on_instructions.append(int(numpy_array[max_light_value_index][2]))
        write_serial(ser,lock_on_instructions)


        ### Scan page format ###
        header = """ <html>
            <head>
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
                background-color: #dddddd;
            }
            #max-table tr:nth-child(even) {background-color: #ffff33;}{background-color: #ffff33;}
            </style>

            <title>Solar Tracker v</title>  
            </head>
            <body>"""
        intro_line = "<h2>Max Light Value:</h2>"

        max_table= """
            <table id="max-table">
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
                <br>
            """ % (numpy_array[max_light_value_index][0], numpy_array[max_light_value_index][1], numpy_array[max_light_value_index][2])

        results_table = """
            <h2>Scan Data:</h2>
            <table>
                <tr>
                    <th>Light Intensity</th>
                    <th>Servo Arm Position</th>
                    <th>Servo Base Position</th>
                </tr>
            """
        # for i in data_list:
        #     results_table_data += """
        #         <tr>
        #             <th>%s</th>
        #             <th>%s</th>
        #             <th>%s</th>
        #         </tr>
        #     """ % (i[0], i[1], i[2])
        for i in numpy_array:
            results_table += """
                <tr>
                    <th>%s</th>
                    <th>%s</th>
                    <th>%s</th>
                </tr>
            """ % (i[0], i[1], i[2])

        results_table += "</table>"

        enchance_button = """<form method="get" action="enhance">
            <button type="submit" style="font-size:15px">Enhance Scan</button>
        </form>"""

        start_over_button = """<form method="get" action="index">
                    <button type="submit" style="font-size:15px">Start Over</button>
                </form>"""

        footer = "</body></html>"
        output = " <br>\n".join([header, intro_line, max_table, results_table, start_over_button, enchance_button, footer])
        return output

    @cherrypy.expose
    def enhance(self, scan_range, precision, x, y):
        ### Enhance page format ###
        header = """ <html>
            <head>
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

if __name__ == "__main__" :
    conf = {'server.socket_host': '0.0.0.0'}
    cherrypy.config.update(conf)
    cherrypy.quickstart(StringGenerator(),config={'/':conf})
