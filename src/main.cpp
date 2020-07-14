#include <iostream>
#include <sstream>

#define LOGGER_DEFINITION
#include <logger.h>

#if _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "display.hpp"
#include "settings.hpp"

#include "pipeline.hpp"

#include "shader.hpp"
#include "camera.hpp"

static const int VERSION_MAJOR = 0;
static const int VERSION_MINOR = 1;
static const int VERSION_PATCH = 0;

void version()
{
	std::stringstream version;

	auto& container = []( std::string s ) { std::string r = ""; for ( auto& c : s ) { r += "-"; } return r; };

	version << "InfernoVX ";
	version << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH;

	std::cout << container( version.str() ) << std::endl;
	std::cout << version.str() << std::endl;
	std::cout << container( version.str() ) << std::endl;
}

void Loop( Display* display )
{
	Logger logger;
	SDL_Event e;

	glm::vec2 resolution = display->GetDisplaySizePx();

	Camera camera { static_cast<int>(resolution.x), static_cast<int>(resolution.y) };
	std::cout << "Camera Position: " << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;

	Pipeline pipeline( display, &camera );

	while ( display->IsWindowOpen )
	{
		// return window size
		bool didResize = display->Input( &e, &camera );
		if ( didResize )
		{
			resolution = display->GetDisplaySizePx();
			camera.UpdateProjection( resolution.x, resolution.y );
			glViewport( 0, 0, resolution.x, resolution.y );
			// notify framebuffer and raytracer
		}

		pipeline.NextFrame( display );

	}

	// cleanup

}

int main( int argc, char** argv )
{
	version();

	Logger mLogger;
	
#ifdef _DEBUG
	mLogger << LOGGER_DEBUG << "Debug mode enabled" << LOGGER_ENDL;
#endif

	// settup display

	std::stringstream version;
	version << "InfernoVX ";
	version << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH;

	Display display { WindowWidth, WindowHeight, version.str() };

	Loop( &display );

	return 0;

}


