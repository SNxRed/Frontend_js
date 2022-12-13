import os
import Carpeta

class Archivo(object):
    def __init__(self, name, path):
        self.name = name
        self.path = path + "/" + self.name
        self.size = os.path.getsize(self.path)
    
    def update_path(self, path):
        self.path = path+'/'+self.name

    def getsize(self):
        return self.size