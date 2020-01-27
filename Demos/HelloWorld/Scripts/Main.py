import Dusk

Dusk.LoadModule("DuskSTBI")
Dusk.LoadModule("DuskTinyOBJ")
# Dusk.LoadModule("DuskFreeType")
Dusk.LoadModule("DuskOpenGL")
# Dusk.LoadModule("DuskVulkan")

gfx = Dusk.GetGraphicsDriver()
gfx.SetWindowSize(( 1024, 768 ))

def onWindowResize(data):
    print(data['Size'])
    print(data['Delta'])

evt = gfx.GetWindowResizedEvent()
dir(evt) # Breaks without this and I have no idea why
evt.AddListener(onWindowResize)

# Dusk.SetRunning(True)
# while Dusk.IsRunning():
#     gfx.ProcessEvents()

#     gfx.SwapBuffers()