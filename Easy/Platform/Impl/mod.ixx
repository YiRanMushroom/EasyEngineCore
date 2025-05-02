export module Easy.Platform.Impl;

#ifdef EZ_RENDERER_OPENGL
export import Easy.Platform.Impl.OpenGL;
#endif

#ifdef EZ_PLATFORM_WINDOWS
export import Easy.Platform.Impl.Windows;
#endif
