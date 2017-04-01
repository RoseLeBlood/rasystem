#!BPY

"""
Name: 'raSystem'
Blender: 242
Group: 'Export'
Tooltip: 'raSystem Geometry Exporter'
"""

import Blender
from Blender import *
import struct
from struct import *

from Blender.BGL import *
from Blender.Draw import *

from os.path import *

# Events
EVENT_NOEVENT = 1
EVENT_EXPORT  = 2
EVENT_QUIT    = 3

def draw():
    global EVENT_NOEVENT,EVENT_EXPORT,EVENT_QUIT
    glClear(GL_COLOR_BUFFER_BIT)

    y = 10
    Button("Export",EVENT_EXPORT , 10, y, 70, 18)
    Button("Quit",EVENT_QUIT, 100, y, 70, 18)
    y += 20

   
    y += 10
    glRasterPos2d(10, y)
    Text("Simple Exporter")
    y += 20
 
def event(evt, val):    
    if (evt == QKEY and not val): 
        Exit()

def bevent(evt):
    global EVENT_NOEVENT,EVENT_EXPORT,EVENT_QUIT
    ######### Manages GUI events
    if (evt == EVENT_QUIT): 
        Exit()
    elif (evt== EVENT_EXPORT):
        fname = Blender.sys.makename(ext=".ram")
        Blender.Window.FileSelector(write, "Export", fname)
#============================================================================#
def writeString(out, s):
    print(s)
    out.write(pack('i', len(s)))
    out.write(s)
#============================================================================#
def write(filename):
    print("\nraSystem Geometry Exporter")
    outfile = file(filename, "wb")
    scn = Blender.Scene.GetCurrent()
    #object = scn.objects.active
    for object in scn.objects:
            print(object.name)
            if object.getType()=='Mesh':		
                    mesh = object.getData(False, True)
                    mesh.quadToTriangle();
                    print(mesh.name)
                    print(len(mesh.verts))
                    print(len(mesh.faces))
                    print(len(mesh.materials))
                    outfile.write(pack('i', len(mesh.verts))) 
                    outfile.write(pack('i', len(mesh.faces)))
                    outfile.write(pack('i', len(mesh.materials)))
                    print("Vertices...")
                    for v in mesh.verts:
                            #outfile.write(pack('3f', v.co.x, v.co.y, v.co.z))
                            outfile.write(pack('3f', v.co.x, v.co.z, v.co.y)) #y und z vertauschen              
                            outfile.write(pack('3f', v.no.x, v.no.y, v.no.z));
                            if mesh.vertexUV:
                                    outfile.write(pack('2f', v.uvco.x, v.uvco.y));
                            else:
                                    outfile.write(pack('2f', 0, 0)); 
                    print("Faces...")
                    for f in mesh.faces:
                            #outfile.write(pack('3H', f.v[0].index, f.v[1].index, f.v[2].index))
                            outfile.write(pack('3H', f.v[0].index, f.v[2].index, f.v[1].index)) # winding order umdrehen
                    print("Materials...")
                    for mat in mesh.materials:
                        if mat != None:
                            print(mat.name)
                            mtextures = mat.getTextures() 
                            #for mtex in mtextures:
                            mtex = mtextures[0];
                            if mtex != None:
                                    print(mtex.tex.name)
                                    if mtex.tex.type == Texture.Types.IMAGE:
                                            #writeString(outfile, mtex.tex.image.filename) # texturdateiname mit pfad
                                            writeString(outfile, split(mtex.tex.image.filename)[1]) #ohne pfad
                                    else:
                                            outfile.write(pack('i', 0)) 		
                            else:
                                    outfile.write(pack('i', 0)) 		
                    print("Subsets...")
                    for f in mesh.faces:
                            print(f.mat)
                            outfile.write(pack('H', f.mat))
                    print("________________________")
    outfile.close()
#============================================================================#
# Einstiegspunkt                                                             #
#                                                                            #
#============================================================================#
#Einfachste Version: 
#write("test.ram")

#Mit Dateiauswahl: 
#fname = Blender.sys.makename(ext=".ram")
#Blender.Window.FileSelector(write, "Export", fname)

#Mit Buttons:
Register(draw, event, bevent)
