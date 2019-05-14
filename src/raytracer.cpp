#include "common.hpp"

#include "image.hpp"
#include "shape.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "plane.hpp"

#ifdef CPORTA
#include "gtest_lite.h"

std::ostream& operator<<(std::ostream& Stream, Vec3 V)
{
    Stream << V.X << ";" << V.Y << ";" << V.Z;
    return Stream;
}

static void RunTests()
{
    GTINIT(std::cin);

    // float es double meret teszt
    // Igazabol static_assert kene hogy legyen a common.hpp-ben
    TEST(Test0, VarTest)
    {
        EXPECT_EQ((size_t)4, sizeof(r32));
        EXPECT_EQ((size_t)8, sizeof(r64));
    }
    ENDM;

    // Matek teszt
    TEST(Test1, Math)
    {
        EXPECT_FLOAT_EQ(0.0f, DegreeToRadian(0.0f)) << "DegreeToRadian hiba\n";
        EXPECT_FLOAT_EQ(Pi32 / 4.0f, DegreeToRadian(45.0f)) << "DegreeToRadian hiba\n";
        EXPECT_FLOAT_EQ(Pi32 / 2.0f, DegreeToRadian(90.0f)) << "DegreeToRadian hiba\n";
        EXPECT_FLOAT_EQ(3.0f * Pi32 / 4.0f, DegreeToRadian(135.0f)) << "DegreeToRadian hiba\n";
        EXPECT_FLOAT_EQ(Pi32, DegreeToRadian(180.0f)) << "DegreeToRadian hiba\n";
        EXPECT_FLOAT_EQ(2.0f*Pi32, DegreeToRadian(360.0f)) << "DegreeToRadian hiba\n";

        EXPECT_FLOAT_EQ(0.0f, RadianToDegree(0.0f)) << "RadianToDegree hiba\n";
        EXPECT_FLOAT_EQ(45.0f, RadianToDegree(Pi32 / 4.0f)) << "RadianToDegree hiba\n";
        EXPECT_FLOAT_EQ(90.0f, RadianToDegree(Pi32 / 2.0f)) << "RadianToDegree hiba\n";
        EXPECT_FLOAT_EQ(135.0f, RadianToDegree(3.0f * Pi32 / 4.0f)) << "RadianToDegree hiba\n";
        EXPECT_FLOAT_EQ(180.0f, RadianToDegree(Pi32)) << "RadianToDegree hiba\n";
        EXPECT_FLOAT_EQ(360.0f, RadianToDegree(2.0f* Pi32)) << "RadianToDegree hiba\n";

        EXPECT_FLOAT_EQ(0.25f, Clamp(-0.1f, 0.25f, 0.75f)) << "Clamp hiba\n";
        EXPECT_FLOAT_EQ(0.25f, Clamp(0.1f, 0.25f, 0.75f)) << "Clamp hiba\n";
        EXPECT_FLOAT_EQ(0.4f, Clamp(0.4f, 0.25f, 0.75f)) << "Clamp hiba\n";
        EXPECT_FLOAT_EQ(0.75f, Clamp(0.9f, 0.25f, 0.75f)) << "Clamp hiba\n";
        EXPECT_FLOAT_EQ(0.75f, Clamp(200.0f, 0.25f, 0.75f)) << "Clamp hiba\n";
        
        Vec3 NullV;
        EXPECT_EQ(NullV, Vec3(0.0f, 0.0f, 0.0f)) << "Vec3 konstruktor hiba\n";
        EXPECT_FLOAT_EQ(0.0f, NullV.LengthSq()) << "Null vektor nem 0?\n";
        EXPECT_FLOAT_EQ(0.0f, NullV.Length()) << "Null vektor nem 0?\n";

        Vec3 V1(0.52f, -0.3f, 12.0f);
        Vec3 V2 = -V1;
        EXPECT_FLOAT_EQ(-V1.X, V2.X) << "Vec3::operator-() hiba\n";
        EXPECT_FLOAT_EQ(-V1.Y, V2.Y) << "Vec3::operator-() hiba\n";
        EXPECT_FLOAT_EQ(-V1.Z, V2.Z) << "Vec3::operator-() hiba\n";

        Vec3 V3(100.0f, 512.0f, -324.12f);

        Vec3 V4 = V1 + V3;
        EXPECT_FLOAT_EQ(V1.X + V3.X, V4.X) << "Vec3::operator+ hiba\n";
        EXPECT_FLOAT_EQ(V1.Y + V3.Y, V4.Y) << "Vec3::operator+ hiba\n";
        EXPECT_FLOAT_EQ(V1.Z + V3.Z, V4.Z) << "Vec3::operator+ hiba\n";

        Vec3 V5 = V1 - V2;
        EXPECT_FLOAT_EQ(V1.X - V2.X, V5.X) << "Vec3::operator- hiba\n";
        EXPECT_FLOAT_EQ(V1.Y - V2.Y, V5.Y) << "Vec3::operator- hiba\n";
        EXPECT_FLOAT_EQ(V1.Z - V2.Z, V5.Z) << "Vec3::operator- hiba\n";

        Vec3 V6 = V1 * V2;
        EXPECT_FLOAT_EQ(V1.X * V2.X, V6.X) << "Vec3::operator* hiba\n";
        EXPECT_FLOAT_EQ(V1.Y * V2.Y, V6.Y) << "Vec3::operator* hiba\n";
        EXPECT_FLOAT_EQ(V1.Z * V2.Z, V6.Z) << "Vec3::operator* hiba\n";

        Vec3 V7 = 52.0f * V1;
        EXPECT_FLOAT_EQ(52.0f * V1.X, V7.X) << "Vec3::operator* hiba\n";
        EXPECT_FLOAT_EQ(52.0f * V1.Y, V7.Y) << "Vec3::operator* hiba\n";
        EXPECT_FLOAT_EQ(52.0f * V1.Z, V7.Z) << "Vec3::operator* hiba\n";

        Vec3 V8 = V1 / 13.0f;
        EXPECT_FLOAT_EQ(V1.X / 13.0f, V8.X) << "Vec3::operator/ hiba\n";
        EXPECT_FLOAT_EQ(V1.Y / 13.0f, V8.Y) << "Vec3::operator/ hiba\n";
        EXPECT_FLOAT_EQ(V1.Z / 13.0f, V8.Z) << "Vec3::operator/ hiba\n";

        EXPECT_ANY_THROW(V1 / 0.0f) << "Vec3::operator/ nem dob 0 eseten\n";

        EXPECT_FLOAT_EQ(144.3604f, V1.LengthSq()) << "Vec3::LengthSq hiba\n";
        EXPECT_FLOAT_EQ(12.0150072f, V1.Length()) << "Vec3::Length hiba\n";

        EXPECT_FLOAT_EQ(1.0f, Normalize(V1).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V2).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V3).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V4).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V5).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V6).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V7).Length()) << "Normalize() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, Normalize(V8).Length()) << "Normalize() hiba\n";

        EXPECT_FLOAT_EQ(0.0f, Dot(NullV, NullV)) << "Dot() hiba\n";
        EXPECT_FLOAT_EQ(V1.LengthSq(), Dot(V1, V1)) << "Dot() hiba\n";
        EXPECT_FLOAT_EQ(V1.X * V3.X + V1.Y * V3.Y + V1.Z * V3.Z, Dot(V1, V3)) << "Dot() hiba\n";

        EXPECT_EQ(NullV, Cross(NullV, V1)) << "Cross() hiba\n";
        EXPECT_EQ(NullV, Cross(NullV, V1)) << "Cross() hiba\n";

        EXPECT_FLOAT_EQ(0.0f, Dot(Cross(V1, V3), V1)) << "Cross() hiba\n";
        //EXPECT_FLOAT_EQ(0.0f, Dot(Cross(V1, V3), V3)) << "Cross() hiba\n"; 
        //^ Ehhez igazabol double kene

        Vec3 Cr1 = Cross(V1, V3);
        Vec3 Cr2 = Cross(V3, V1);
        EXPECT_FLOAT_EQ(Cr1.X, -Cr2.X) << "Cross() hiba\n";
        EXPECT_FLOAT_EQ(Cr1.Y, -Cr2.Y) << "Cross() hiba\n";
        EXPECT_FLOAT_EQ(Cr1.Z, -Cr2.Z) << "Cross() hiba\n";

        EXPECT_EQ(Vec3(1.0f, 0.0f, 0.0f), 
                  Project(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f))) << "Project() hiba\n";
        EXPECT_EQ(Vec3(0.0f, 1.0f, 0.0f),
                  Project(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f))) << "Project() hiba\n";
        EXPECT_EQ(Vec3(0.0f, 0.0f, 1.0f),
                  Project(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f))) << "Project() hiba\n";

        EXPECT_EQ(Vec3(0.0f, 1.0f, 1.0f),
                  Reject(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f))) << "Reject() hiba\n";
        EXPECT_EQ(Vec3(1.0f, 0.0f, 1.0f),
                  Reject(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f))) << "Reject() hiba\n";
        EXPECT_EQ(Vec3(1.0f, 1.0f, 0.0f),
                  Reject(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f))) << "Reject() hiba\n";

        EXPECT_EQ(Vec3(-1.0f, 0.5f, 0.0f), Reflect(Vec3(-1.0f, -0.5f, 0.0f), Vec3(0.0f, 1.0f, 0.0f))) << "Reflect() hiba\n";
    }
    ENDM;

    // Alapveto sugar-test intersect teszt
    TEST(Test2, Intersect)
    {
        CSphere Sphere(Vec3(0.0f, 0.0f, -2.0f), 1.0f);
        CPlane Plane(Vec3(0.0f, 1.0f, 0.0f), -1.0f);

        CRay Ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        SHitInfo HitInfo = {};

        EXPECT_TRUE(Sphere.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CSphere::Intersect() hiba\n";
        EXPECT_FLOAT_EQ(1.0f, HitInfo.tVal) << "CSphere::Intersect() HitInfo hiba\n";

        EXPECT_FALSE(Plane.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CPlane::Intersect() hiba\n";

        Ray = CRay(Vec3(0.0f, 0.0f, 0.0f), Normalize(Vec3(0.0f, -1.0f, -1.0f)));
        
        EXPECT_FALSE(Sphere.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CSphere::Intersect() hiba\n";

        EXPECT_TRUE(Plane.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CPlane::Intersect() hiba\n";
        EXPECT_FLOAT_EQ(sqrtf(2.0f), HitInfo.tVal) << "CPlane::Intersect() hiba\n";
    }
    ENDM;

    // Teszt: A HeteroStore-t teszteljuk, hogy helyes-e az elemszam, stb
    TEST(Test2, Hetero)
    {
        CHeteroStore<IShape> Store;
        EXPECT_EQ((size_t)0, Store.Size()) << "CHeteroStore::Size() hiba\n";

        EXPECT_ANY_THROW(Store[2]) << "CHeteroStore::operator[] nem dob\n";

        Store.PushBack(std::make_shared<CPlane>(Vec3(0.0f, 1.0f, 0.0f), -1.0f));
        Store.PushBack(std::make_shared<CSphere>(Vec3(0.0f, 0.0f, -1.0f), 1.0f));

        EXPECT_EQ((size_t)2, Store.Size()) << "CHeteroStore::Size() hiba\n";

        EXPECT_NO_THROW(Store[1]) << "CHeteroStore::operator[] dob\n";
        EXPECT_ANY_THROW(Store[2]) << "CHeteroStore::operator[] nem dob\n";
        EXPECT_ANY_THROW(Store[12]) << "CHeteroStore::operator[] nem dob\n";
    }
    ENDM;

    // Teszt: hogy bizonyos sugarak helyes objektumokat talalnek-e el
    TEST(Test3, Scene)
    {
        CScene Scene;

        CCamera Camera;

        std::shared_ptr<CSphere> Sphere = std::make_shared<CSphere>(Vec3(0.0f, 0.0f, -2.0f), 1.0f);
        std::shared_ptr<CPlane> Plane   = std::make_shared<CPlane>(Vec3(0.0f, 1.0f, 0.0f), -1.0f);

        Scene.AddShape(Sphere);
        Scene.AddShape(Plane);

        CRay Ray = Camera.RayFromUV(0.0f, 0.0f);

        SHitInfo HitInfo;
        EXPECT_TRUE(Scene.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CScene::Intersect() hiba\n";
        EXPECT_EQ(Sphere.get(), HitInfo.Shape) << "CScene::Intersect() hiba\n";

        Ray = Camera.RayFromUV(-0.5f, 0.0f);
        EXPECT_TRUE(Scene.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CScene::Intersect() hiba\n";
        EXPECT_EQ(Sphere.get(), HitInfo.Shape) << "CScene::Intersect() hiba\n";

        Ray = Camera.RayFromUV(-1.0f, 0.0f);
        EXPECT_FALSE(Scene.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CScene::Intersect() hiba\n";

        Ray = Camera.RayFromUV(0.0f, -0.5f);
        EXPECT_TRUE(Scene.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CScene::Intersect() hiba\n";
        EXPECT_EQ(Sphere.get(), HitInfo.Shape) << "CScene::Intersect() hiba\n";

        Ray = Camera.RayFromUV(0.0f, -1.0f);
        EXPECT_TRUE(Scene.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CScene::Intersect() hiba\n";
        EXPECT_EQ(Plane.get(), HitInfo.Shape) << "CScene::Intersect() hiba\n";

        Ray = Camera.RayFromUV(0.0f, 0.75f);
        EXPECT_FALSE(Scene.Intersect(Ray, 0.0f, FLT_MAX, HitInfo)) << "CScene::Intersect() hiba\n";
        
    }
    ENDM;

    GTEND(std::cerr);
}

int main(int argc, char** argv)
{
    RunTests();
    return 0;
}


#else

/** Displays the command line usage for the app.
*/
static void PrintHelp()
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
static bool ParseArguments(int argc, char** argv, SArguments& Arguments)
{
    //std::string CurrentArg = "";
    for(int i = 1; i < argc; i++)
    {
        std::string Arg = argv[i];
        if(Arg[0] == '-')
        {
            Arg.erase(0, 1);
            std::string Type = Arg.substr(0, Arg.find_first_not_of("abcdefghijklmnopqrstvuxyz? ", 0));
            Arg.erase(0, Type.size());

            if(Type == "res")
            {
                size_t DelimPos = Arg.find('x', 0);
                if(DelimPos == std::string::npos)
                {
                    throw std::invalid_argument("Invalid resolution");
                }

                std::string Width = Arg.substr(0, DelimPos);
                std::string Height = Arg.substr(DelimPos + 1);

                Arguments.RenderWidth = std::stoi(Width);
                Arguments.RenderHeight = std::stoi(Height);

                if(Arguments.RenderWidth < 0 || Arguments.RenderHeight < 0)
                {
                    throw std::invalid_argument("Negative resolution");
                }

            }
            else if(Type == "threads")
            {
                Arguments.MaxThreadCount = std::stoul(Arg);
                if(Arguments.MaxThreadCount == 0)
                {
                    std::invalid_argument("0 threads is not allowed");
                }
            }
            else if(Type == "samples")
            {
                Arguments.SampleCount = std::stoul(Arg);

            }
            else if(Type == "depth")
            {
                Arguments.MaxDepth = std::stoul(Arg);

            }
            else if(Type == "?")
            {
                PrintHelp();
                return false;
            }
            else
            {
                throw std::invalid_argument("Unknown command");
            }
        }
        else
        {
            if(i == 1)
            {
                Arguments.OutputName = Arg;
            }
            else if(i == 2)
            {
                Arguments.ScenePath = Arg;
            }
            else
            {
                throw std::invalid_argument("Unknown command");
            }
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
    Arguments.RenderWidth = 100;
    Arguments.RenderHeight = 75;
    Arguments.SampleCount = 4;
    Arguments.MaxDepth = UINT32_MAX;
    Arguments.MaxThreadCount = 16;

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
        Scene.Read(TestJSON);
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
    Image.Write(File);
    return 0;
}
#endif