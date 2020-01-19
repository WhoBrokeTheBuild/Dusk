import Dusk

Dusk.LoadModule("DuskSTBI")
Dusk.LoadModule("DuskTinyOBJ")
# Dusk.LoadModule("DuskFreeType")
Dusk.LoadModule("DuskOpenGL")
# Dusk.LoadModule("DuskVulkan")

gfx = Dusk.GetGraphicsDriver()
gfx.SetWindowSize(( 1024, 768 ))

# Dusk.SetRunning(True)
# while Dusk.IsRunning():
#     gfx.ProcessEvents()

#     gfx.SwapBuffers()