#include "Model.h"
#include <fstream>

Model::Model() : shader("data/shaders/shad_ambient.vs", "data/shaders/shad_ambient.fs")
{
}

Model::~Model()
{
}

void Model::init()
{
}

void Model::update()
{
}

void Model::render()
{
}

Shader Model::getShader()
{
    return shader;
}

void Model::setShader(Shader& _shader)
{
    shader = _shader;
}

glm::vec3 Model::cameraPos = glm::vec3(0.f, 0.f, 0.f);
glm::mat4 Model::projection = glm::mat4();
glm::mat4 Model::view = glm::mat4();

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int Model::loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int Model::loadTexture(char const* path, int* _width, int* _height)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int nrComponents;
    unsigned char* data = stbi_load(path, _width, _height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, *_width, *_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


//// utility function for loading a 2D texture from file using libpng
//// ---------------------------------------------------
//unsigned int Model::loadTexture2(char const* path, int* _width, int* _height)
//{
//    unsigned int textureID = 0;
//
//    //following the manual
//    //-----------------------------------------------------
//
//    /*
//    You will want to do the I/O initialization(*) before you get into libpng, so if it doesn't work, you don't have much to undo.
//    Of course, you will also want to insure that you are, in fact, dealing with a PNG file.
//    Libpng provides a simple check to see if a file is a PNG file.
//    To use it, pass in the first 1 to 8 bytes of the file to the function png_sig_cmp(), and it will return 0 if the bytes match the
//    corresponding bytes of the PNG signature, or nonzero otherwise. Of course, the more bytes you pass in, the greater the accuracy of the prediction.
//
//    If you are intending to keep the file pointer open for use in libpng, you must ensure you don't read more than 8 bytes from the beginning of the file,
//    and you also have to make a call to png_set_sig_bytes_read() with the number of bytes you read from the beginning.
//    xLibpng will then only check the bytes (if any) that your program didn't read.
//    */
//    std::ifstream filestream;
//    FILE* fp = fopen_s(path, "rb");
//    if (!fp)
//    {
//        return (0);
//    }
//    fread(fp, 1, 8, fp);
//    bool is_png = !png_sig_cmp((png_const_bytep)fp, 0, 8);
//    if (!is_png)
//    {
//        return (0);
//    }
//
//    /*
//    Next, png_struct and png_info need to be allocated and initialized.
//    In order to ensure that the size of these structures is correct even with a dynamically linked libpng, there are functions to initialize and allocate the structures.
//    We also pass the library version, optional pointers to error handling functions, and a pointer to a data struct for use by the error functions,
//    if necessary (the pointer and functions can be NULL if the default error handlers are to be used). See the section on Changes to Libpng below regarding the
//    old initialization functions. The structure allocation functions quietly return NULL if they fail to create the structure, so your application should check for that.
//    */
//    png_structp png_ptr = png_create_read_struct
//    (PNG_LIBPNG_VER_STRING, (png_voidp)nullptr,
//        nullptr, nullptr);
//    if (!png_ptr)
//        return (0);
//
//    png_infop info_ptr = png_create_info_struct(png_ptr);
//    if (!info_ptr)
//    {
//        png_destroy_read_struct(&png_ptr,
//            (png_infopp)NULL, (png_infopp)NULL);
//        return (0);
//    }
//
//    png_infop end_info = png_create_info_struct(png_ptr);
//    if (!end_info)
//    {
//        png_destroy_read_struct(&png_ptr, &info_ptr,
//            (png_infopp)NULL);
//        return (0);
//    }
//
//    /*
//    The error handling routines passed to png_create_read_struct() and the memory alloc/free routines passed to png_create_struct_2() are only necessary if
//    you are not using the libpng supplied error handling and memory alloc/free functions.
//
//    When libpng encounters an error, it expects to longjmp back to your routine. Therefore, you will need to call setjmp and pass your png_jmpbuf(png_ptr).
//    If you read the file from different routines, you will need to update the jmpbuf field every time you enter a new routine that will call a png_*() function.
//
//    See your documentation of setjmp/longjmp for your compiler for more information on setjmp/longjmp.
//    See the discussion on libpng error handling in the Customizing Libpng section below for more information on the libpng error handling.
//    If an error occurs, and libpng longjmp's back to your setjmp, you will want to call png_destroy_read_struct() to free any memory.
//    */
//    if (setjmp(png_jmpbuf(png_ptr)))
//    {
//        png_destroy_read_struct(&png_ptr, &info_ptr,
//            &end_info);
//        fclose(fp);
//        return (0);
//    }
//
//    /*
//    If you would rather avoid the complexity of setjmp/longjmp issues, you can compile libpng with PNG_SETJMP_NOT_SUPPORTED,
//    in which case errors will result in a call to PNG_ABORT() which defaults to abort().
//
//    Now you need to set up the input code. The default for libpng is to use the C function fread().
//    If you use this, you will need to pass a valid FILE * in the function png_init_io().
//    Be sure that the file is opened in binary mode. If you wish to handle reading data in another way, you need not call the png_init_io() function,
//    but you must then implement the libpng I/O methods discussed in the Customizing Libpng section below.
//    */
//    png_init_io(png_ptr, fp);
//
//    /*
//    If you had previously opened the file and read any of the signature from the beginning in order to see if this was a PNG file,
//    you need to let libpng know that there are some bytes missing from the start of the file.
//    */
//    png_set_sig_bytes(png_ptr, 8);
//    //------------------------------------------------
//
//
//    return textureID;
//}
