import os
import Carpeta as Fo
import Archivo as Fi

def commands(puntero, comando):
    comando = comando.split(" ")
    try:
        if(comando[0]=="ls"):
            for i in puntero.ls():
                print(i)
        elif(comando[0] == "touch"):
            nombre = comando[1]
            puntero.touch(nombre)
            
        elif(comando[0] == "mkdir"):
            nombre = comando[1]
            puntero.mkdir(nombre)
            
        elif(comando[0] == "mv"):
            src = comando[1]
      
            dst = comando[2]
            print(puntero.move(src,dst))
        elif(comando[0] == "rm"):
            src = comando[1]
     
            print(puntero.rm(src))
        elif(comando[0] == "rmdir"):
            src = comando[1]
         
            print(puntero.rmdir(src))
        elif(comando[0]=="ls-R"):
            for i in puntero.lsr():       
                print(i)    
        elif(comando[0] =="ls-i"):
             puntero.lsi()
            
                
    except:
        pass    


if __name__ == "__main__":
    
    origen = os.path.join(os.getcwd(), os.listdir(os.getcwd())[0])
   

    carpetap = Fo.Folder("CarpetaP",os.path.dirname(origen))

    puntero = carpetap
    
    com = str(input(puntero.path+"# "))
    while(1):
        comand = com.split(" ")
        if(com == "exit"):
            break
        elif(comand[0]=="cd"):
            nombre = comand[1]
            if(nombre == ".."):
                if(puntero.index):
                    puntero = puntero.index
            elif(nombre in puntero.child):
                puntero = puntero.child[nombre]
        else:
            commands(puntero, com)
        com = str(input(puntero.path + "# "))
    
    carpetap.delete_all()