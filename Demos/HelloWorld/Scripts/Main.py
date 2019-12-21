import Dusk

Dusk.SetApplicationName("HelloWorld")
Dusk.SetApplicationVersion(( 1, 0, 0 ))

Dusk.LoadModule("DuskSTBI")
Dusk.LoadModule("DuskTinyOBJ")
Dusk.LoadModule("DuskFreeType")
Dusk.LoadModule("DuskOpenGL")

gfx = Dusk.GetGraphicsDriver()
gfx.SetWindowSize(( 1024, 768 ))

Dusk.SetRunning(True)
while Dusk.IsRunning():
    gfx.ProcessEvents()

    gfx.SwapBuffers()