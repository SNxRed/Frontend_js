import os, shutil
import Archivo as F

mode = 0o777

class Folder(object):
    def __init__(self, nombre, path, index = None):
        self.index = None
        if(index):
            self.index = index
        self.path_index = path
        self.path = path+"/"+nombre
        if not os.path.exists(self.path):
            try:
                os.mkdir(self.path, mode)    
            except:
                raise OSError("no se puede acceder")
        self.child = {} #carpetas 
        self.files = {} #archivos

    def busqueda(self, dst):
        pasos = dst.split("/")
        #arr = pasos[:1]
        aux = self
        while(aux.index):
            aux = aux.index
        
        pasos = pasos[6:]
        for i in pasos:
            if(i in aux.child):
                aux = aux.child[i]
            else:
                return None
        return aux

    def ls(self):
        return os.listdir(self.path)
    
    def lsi(self):
        list = []
        for i in self.child:
            list.append(i)
        
        for i in self.files:
            list.append(i)
       # aux = self.path
        for i in list:
            try:
                self.path = self.child[i].self.path   
                print(str(os.stat(self.path).st_ino)+" "+i)
            except:
                self.path = self.files[i].self.path
                print(str(os.stat(self.path).st_ino)+" "+i)        
    
    def lsr(self):
        for root, dirs, files in os.walk(self.path, topdown=False):
                for name in files:
                    print(os.path.join(root, name))
                for name in dirs:
                    print(os.path.join(root, name))      
               
    def mkdir(self, nombre):
        nuevo = Folder(nombre, self.path, self)
        self.child[nombre] = nuevo
        if not os.path.exists(self.path):
            try:
                os.mkdir(self.path+'/'+nombre, mode)  
            except:
                raise OSError("no se puede crear archivos")
        
    
    def touch(self, name):
        if os.path.exists(name):
            os.utime(self.path+"/"+name, None)
        else:
            open(self.path+"/"+name, 'a').close()
        
        nuevo = F.Archivo(name,self.path)
        self.files[name] = nuevo
       

    def move(self,src,dst):
        archivo = None 
        if(src in self.files):
            archivo = self.files[src]
        else:
            return False
        dest = self.busqueda(dst)
        if(dest):
            dest.files[src] = archivo
        else:
            return False
        
        self.files.pop(src)
        shutil.move(self.path+"/"+src,dst)
        archivo.update_path(dst)
        return True

    def delete_all(self):
        while(len(self.child) > 0):
            nom_a = list(self.child.keys())[0]
            aux = self.child[nom_a]
            if(len(aux.child) > 0 or len(aux.files) > 0):
                aux.delete_all()
            os.rmdir(aux.path)
            self.child.pop(nom_a)
            aux = None

        while(len(self.files) > 0):
            nom_a = list(self.files.keys())[0]
            aux = self.files[nom_a]
            os.remove(aux.path)
            self.files.pop(nom_a)
            aux = None

    def rmdir(self,key): #key = nombre del archivo que se desea eliminar
        print(self.child)
        if(key in self.child):
            aux = self.child[key]
            aux.delete_all()
            os.rmdir(aux.path)
            self.child.pop(key)
            aux = None
        else:
            print("El directorio no existe")

    
    def rm(self,key):
        self.files.pop(key)
        os.remove(self.path+"/"+str(key))
        