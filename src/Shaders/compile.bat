forfiles /P A:\Repos\Zero_Engine\src\Shaders\ /M *.vert /C "CMD /C glslc @FILE -o @FILE.spv"
forfiles /P A:\Repos\Zero_Engine\src\Shaders\ /M *.frag /C "CMD /C glslc @FILE -o @FILE.spv"