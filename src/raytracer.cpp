#include "common.hpp"

#include "image.hpp"
#include "shape.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "plane.hpp"

/** Displays the command line usage for the app.
*/
void PrintHelp()
{
    std::cout << "Usage: [output-filename] [scene-filename] [options]\n\n" <<
        "Options:\n" <<
        "  -res\t<uint>x<uint>\t" << "Specifies the resolution (width*height) of the output file.\n" <<
        "  -samples\t<uint>\t" << "Specifies the number of rays to shoot per pixel.\n" <<
        "  -threads\t<uint>\t" << "Specifies the maximum number of threads the app can use.\n" <<
        "  -depth\t<uint>\t" << "Specifies the maximum number of bounces a ray can bounce.\n" <<
        "  -?\t" << "Prints the help information.\n";
}

/** Contains all the possible arguments the program can start with
*/
struct SArguments
{
    std::string OutputName; /**< The path of the output image file. */
    std::string ScenePath; /**< Path to the scene to load (json format). */
    s32 RenderWidth; /**< Width of the output image. */
    s32 RenderHeight; /**< Height of the output image. */
    u32 SampleCount; /**< Number of rays to shoot per pixel. */
    u32 MaxDepth; /**< Maximum number of times a ray can bounce. */
    u32 MaxThreadCount; /**< Maximum number of threads the app is allowed to create. */
};

/** Parses the command line arguments.
*/
bool ParseArguments(int argc, char** argv, SArguments& Arguments)
{
    std::string CurrentArg = "";
    for(int i = 1; i < argc; i++)
    {
        if(CurrentArg == "")
        {
            CurrentArg = argv[i];
            if(i == 1)
            {
                Arguments.OutputName = argv[i];
            }
            else if(i == 2)
            {
                Arguments.ScenePath = argv[i];
            }
            else
            {
                continue;
            }
        }
        else if(CurrentArg == "-res")
        {
            std::string Resolution = argv[i];
            size_t DelimPos = Resolution.find('x', 0);
            if(DelimPos == std::string::npos)
            {
                throw std::invalid_argument("Invalid resolution");
            }

            std::string Width = Resolution.substr(0, DelimPos);
            std::string Height = Resolution.substr(DelimPos + 1);

            Arguments.RenderWidth = std::stoi(Width);
            Arguments.RenderHeight = std::stoi(Height);

            if(Arguments.RenderWidth < 0 || Arguments.RenderHeight < 0)
            {
                throw std::invalid_argument("Negative resolution");
            }
            
        }
        else if(CurrentArg == "-threads")
        {
            std::string Threads = argv[i];

            Arguments.MaxThreadCount = std::stoul(Threads);
            if(Arguments.MaxThreadCount == 0)
            {
                std::invalid_argument("0 threads");
            }
        }
        else if(CurrentArg == "-samples")
        {
            std::string Samples = argv[i];
            Arguments.SampleCount = std::stoul(Samples);
            
        }
        else if(CurrentArg == "-depth")
        {
            std::string Depth = argv[i];
            Arguments.MaxDepth = std::stoul(Depth);
            
        }
        else if(CurrentArg == "-?")
        {
            PrintHelp();
            return false;
        }
        else
        {
            throw std::invalid_argument("Unknown command");
        }

        CurrentArg = "";
    }

    if(CurrentArg != "")
    {
        if(CurrentArg == "-?")
        {
            PrintHelp();
            return false;
        }
        else
        {
            throw std::invalid_argument("Invalid command usage");
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    /* Default values */
    SArguments Arguments;
    Arguments.OutputName = "out.bmp";
    Arguments.ScenePath = "scene.json";
    Arguments.RenderWidth = 1920;
    Arguments.RenderHeight = 1080;
    Arguments.SampleCount = 2048;
    Arguments.MaxDepth = UINT32_MAX;
    Arguments.MaxThreadCount = 1024;

    /* Parsing arguments */
    try
    {
        if(!ParseArguments(argc, argv, Arguments))
        {
            return 0;
        }
    } 
    catch(std::exception Exception)
    {
        std::cout << "Syntax error\n";
        std::cout << Exception.what() << '\n';
        return -1;
    }
    catch(...)
    {
        std::cout << "Syntax error\n";
        return -1;
    }

    CScene Scene;
    
    /* Loading scene */
    std::fstream TestJSON(Arguments.ScenePath, std::ios_base::in);
    if(!TestJSON.is_open())
    {
        std::cout << "Couldn't open file " << Arguments.ScenePath << std::endl;
        return -1;
    }

    try
    {
        TestJSON >> &Scene;
    }
    catch(std::exception Exception)
    {
        std::cout << "Invalid scene file\n";
        std::cout << Exception.what();
        return -1;
    }
    catch(...)
    {
        std::cout << "Invalid scene file\n";
        return -1;
    }

    /* Creating backbuffer */
    CImage Image(Arguments.RenderWidth, Arguments.RenderHeight);

    /* Rendering */
    SRenderParams Params;
    Params.SampleCount = Arguments.SampleCount;
    Params.MaxDepth = Arguments.MaxDepth;
    Params.MaxThreadCount = Arguments.MaxThreadCount;
    Scene.Render(Image, Params);

    /* Writing bmp */
    std::fstream File(Arguments.OutputName, 
                      std::ios_base::out | 
                      std::ios_base::binary | 
                      std::ios_base::trunc);
    File << &Image;

    return 0;
}