import Dusk

Dusk.LoadModule("DuskSTBI")
Dusk.LoadModule("DuskTinyOBJ")
# Dusk.LoadModule("DuskGLTF2")
# Dusk.LoadModule("DuskFreeType")
# Dusk.LoadModule("DuskOpenGL")
Dusk.LoadModule("DuskVulkan")

gfx = Dusk.GetGraphicsDriver()
if gfx == None:
    quit()

gfx.SetWindowSize(( 1024, 768 ))

# def onWindowResize(data):
#     print(data['Size'])

# gfx.WindowResizedEvent.AddListener(onWindowResize)

# Dusk.SetRunning(True)
# while Dusk.IsRunning():
#     gfx.ProcessEvents()

#     gfx.SwapBuffers()