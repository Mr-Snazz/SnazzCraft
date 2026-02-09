import tkinter
from tkinter import *

import subprocess

ExecutableDirectory = "/bin/"

Root = tkinter.Tk()
Root.title("Launcher")
Root.geometry("1200x800")

#Root.mainloop()

subprocess.run(["." + ExecutableDirectory + "executable"], cwd="SnazzCraft", check=True)






