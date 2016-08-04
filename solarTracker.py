# Zack Endsley
# ECE492
# 2016_08_03
# This cherrypy server is designed to be the UI component for the solar tracker project

import cherrypy
import serial
import time, os, glob

PORT = '/dev/tty.usbmodem1451'

ser = serial.Serial(
    port=PORT,
    baudrate=9600
    )

# Saves raw input from serial stream to list; each list member is length of the specified byte size
# Assumes serial_feed is already .open() thus live
def read_serial(live_serial_feed, bytes=5):
    serial_data = []
    while live_serial_feed.in_waiting > 0 :
        serial_data.append(live_serial_feed.read(bytes))
    return serial_data

# Writes given output to a live serial feed in hex bytes from given data list
def write_serial(live_serial_feed, data):
    for i in data:
        print i
        live_serial_feed.write(i)
    return



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
    def scan(self, select=1, scan_range=90, precision=10, x=0, y=0):
        #data_points = sweep_sky(precision)
        #max_light_point = find_max_point(data_points)
        select=1
        scan_range=90
        x=0
        y=0
        max_light_point = [450,20,60]
        scan_data = []
        scan_data.append(select)
        scan_data.append(scan_range)
        scan_data.append(precision)
        scan_data.append(x)
        scan_data.append(y)
        write_serial(ser,scan_data)

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
