#include "Model.h"
#include <fstream>
#include <string>
#include <iostream>

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
//unsigned int Model::loadTexture(char const* path, int* _width, int* _height)
//{
//    int y;
//    unsigned int sig_read = 0;
//    int bit_depth, color_type, interlace_type;
//    int num_palette;
//    png_colorp palette;
//    png_bytepp rows_pointer;
//
//    // Read config.json
//    std::ifstream configfile(path, std::ios::out | std::ios::app | std::ios::binary);
//
//    /* Create and initialize the png_struct with the desired error handler
//     * functions.  If you want to use the default stderr and longjump method,
//     * you can supply NULL for the last three parameters.  We also supply the
//     * the compiler header file version, so that we know if the application
//     * was compiled with a compatible version of the library
//     */
//    png_structp png =
//        png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
//            report_png_error, report_png_error);
//    if (png == nullptr) {
//        std::cout << "png_create_read_struct" << std::endl;
//        return false;
//    }
//
//    // Do don't check if the sRGB color profile is valid, it gives
//  // problems with sRGB IEC61966-2.1 color profile from Photoshop.
//  // See this thread: https://community.aseprite.org/t/2656
//    png_set_option(png, PNG_SKIP_sRGB_CHECK_PROFILE, PNG_OPTION_ON);
//
//    //// Set a function to read user data chunks
//    //auto opts = std::make_shared<PngOptions>();
//    //png_set_read_user_chunk_fn(png, &opts, png_user_chunk);
//
//    /* Allocate/initialize the memory for image information. */
//    png_infop info = png_create_info_struct(png);
//    /*DestroyReadPng destroyer(png, info);
//    if (info == nullptr) {
//        fop->setError("png_create_info_struct\n");
//        return false;
//    }*/
//
//    ///* Set error handling if you are using the setjmp/longjmp method (this is
//    // * the normal method of doing things with libpng).
//    // */
//    //if (setjmp(png_jmpbuf(png))) {
//    //    fop->setError("Error reading PNG file\n");
//    //    return false;
//    //}
//
//    ///* Set up the input control if you are using standard C streams */
//    //png_init_io(png, fp);
//
//    ///* If we have already read some of the signature */
//    //png_set_sig_bytes(png, sig_read);
//
//    ///* The call to png_read_info() gives us all of the information from the
//    // * PNG file before the first IDAT (image data chunk).
//    // */
//    //png_read_info(png, info);
//
//    //png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type,
//    //    &interlace_type, NULL, NULL);
//
//
//    ///* Set up the data transformations you want.  Note that these are all
//    // * optional.  Only call them if you want/need them.  Many of the
//    // * transformations only work on specific types of images, and many
//    // * are mutually exclusive.
//    // */
//
//    // /* tell libpng to strip 16 bit/color files down to 8 bits/color */
//    //png_set_strip_16(png);
//
//    ///* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
//    // * byte into separate bytes (useful for paletted and grayscale images).
//    // */
//    //png_set_packing(png);
//
//    ///* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
//    //if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
//    //    png_set_expand_gray_1_2_4_to_8(png);
//
//    ///* Turn on interlace handling.  REQUIRED if you are not using
//    // * png_read_image().  To see how to handle interlacing passes,
//    // * see the png_read_row() method below:
//    // */
//    //int number_passes = png_set_interlace_handling(png);
//
//    ///* Optional call to gamma correct and add the background to the palette
//    // * and update info structure.
//    // */
//    //png_read_update_info(png, info);
//
//    ///* create the output image */
//    //switch (png_get_color_type(png, info)) {
//
//    //case PNG_COLOR_TYPE_RGB_ALPHA:
//    //    fop->sequenceSetHasAlpha(true);
//    //case PNG_COLOR_TYPE_RGB:
//    //    pixelFormat = IMAGE_RGB;
//    //    break;
//
//    //case PNG_COLOR_TYPE_GRAY_ALPHA:
//    //    fop->sequenceSetHasAlpha(true);
//    //case PNG_COLOR_TYPE_GRAY:
//    //    pixelFormat = IMAGE_GRAYSCALE;
//    //    break;
//
//    //case PNG_COLOR_TYPE_PALETTE:
//    //    pixelFormat = IMAGE_INDEXED;
//    //    break;
//
//    //default:
//    //    fop->setError("Color type not supported\n)");
//    //    return false;
//    //}
//
//    //int imageWidth = png_get_image_width(png, info);
//    //int imageHeight = png_get_image_height(png, info);
//    //Image* image = fop->sequenceImage(pixelFormat, imageWidth, imageHeight);
//    //if (!image) {
//    //    fop->setError("file_sequence_image %dx%d\n", imageWidth, imageHeight);
//    //    return false;
//    //}
//
//    //// Transparent color
//    //png_color_16p png_trans_color = NULL;
//
//    //// Read the palette
//    //if (png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE &&
//    //    png_get_PLTE(png, info, &palette, &num_palette)) {
//    //    fop->sequenceSetNColors(num_palette);
//
//    //    for (int c = 0; c < num_palette; ++c) {
//    //        fop->sequenceSetColor(c,
//    //            palette[c].red,
//    //            palette[c].green,
//    //            palette[c].blue);
//    //    }
//
//    //    // Read alpha values for palette entries
//    //    png_bytep trans = NULL;     // Transparent palette entries
//    //    int num_trans = 0;
//    //    int mask_entry = -1;
//
//    //    png_get_tRNS(png, info, &trans, &num_trans, nullptr);
//
//    //    for (int i = 0; i < num_trans; ++i) {
//    //        fop->sequenceSetAlpha(i, trans[i]);
//
//    //        if (trans[i] < 255) {
//    //            fop->sequenceSetHasAlpha(true); // Is a transparent sprite
//    //            if (trans[i] == 0) {
//    //                if (mask_entry < 0)
//    //                    mask_entry = i;
//    //            }
//    //        }
//    //    }
//
//    //    if (mask_entry >= 0)
//    //        fop->document()->sprite()->setTransparentColor(mask_entry);
//    //}
//    //else {
//    //    png_get_tRNS(png, info, nullptr, nullptr, &png_trans_color);
//    //}
//
//    //// Allocate the memory to hold the image using the fields of info.
//    //rows_pointer = (png_bytepp)png_malloc(png, sizeof(png_bytep) * height);
//    //for (y = 0; y < height; y++)
//    //    rows_pointer[y] = (png_bytep)png_malloc(png, png_get_rowbytes(png, info));
//
//    //for (int pass = 0; pass < number_passes; ++pass) {
//    //    for (y = 0; y < height; y++) {
//    //        png_read_rows(png, rows_pointer + y, nullptr, 1);
//
//    //        fop->setProgress(
//    //            (double)((double)pass + (double)(y + 1) / (double)(height))
//    //            / (double)number_passes);
//
//    //        if (fop->isStop())
//    //            break;
//    //    }
//    //}
//
//    //// Convert rows_pointer into the doc::Image
//    //for (y = 0; y < height; y++) {
//    //    // RGB_ALPHA
//    //    if (png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB_ALPHA) {
//    //        uint8_t* src_address = rows_pointer[y];
//    //        uint32_t* dst_address = (uint32_t*)image->getPixelAddress(0, y);
//    //        unsigned int x, r, g, b, a;
//
//    //        for (x = 0; x < width; x++) {
//    //            r = *(src_address++);
//    //            g = *(src_address++);
//    //            b = *(src_address++);
//    //            a = *(src_address++);
//    //            *(dst_address++) = rgba(r, g, b, a);
//    //        }
//    //    }
//    //    // RGB
//    //    else if (png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB) {
//    //        uint8_t* src_address = rows_pointer[y];
//    //        uint32_t* dst_address = (uint32_t*)image->getPixelAddress(0, y);
//    //        unsigned int x, r, g, b, a;
//
//    //        for (x = 0; x < width; x++) {
//    //            r = *(src_address++);
//    //            g = *(src_address++);
//    //            b = *(src_address++);
//
//    //            // Transparent color
//    //            if (png_trans_color &&
//    //                r == png_trans_color->red &&
//    //                g == png_trans_color->green &&
//    //                b == png_trans_color->blue) {
//    //                a = 0;
//    //                if (!fop->sequenceGetHasAlpha())
//    //                    fop->sequenceSetHasAlpha(true);
//    //            }
//    //            else
//    //                a = 255;
//
//    //            *(dst_address++) = rgba(r, g, b, a);
//    //        }
//    //    }
//    //    // GRAY_ALPHA
//    //    else if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY_ALPHA) {
//    //        uint8_t* src_address = rows_pointer[y];
//    //        uint16_t* dst_address = (uint16_t*)image->getPixelAddress(0, y);
//    //        unsigned int x, k, a;
//
//    //        for (x = 0; x < width; x++) {
//    //            k = *(src_address++);
//    //            a = *(src_address++);
//    //            *(dst_address++) = graya(k, a);
//    //        }
//    //    }
//    //    // GRAY
//    //    else if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY) {
//    //        uint8_t* src_address = rows_pointer[y];
//    //        uint16_t* dst_address = (uint16_t*)image->getPixelAddress(0, y);
//    //        unsigned int x, k, a;
//
//    //        for (x = 0; x < width; x++) {
//    //            k = *(src_address++);
//
//    //            // Transparent color
//    //            if (png_trans_color &&
//    //                k == png_trans_color->gray) {
//    //                a = 0;
//    //                if (!fop->sequenceGetHasAlpha())
//    //                    fop->sequenceSetHasAlpha(true);
//    //            }
//    //            else
//    //                a = 255;
//
//    //            *(dst_address++) = graya(k, a);
//    //        }
//    //    }
//    //    // PALETTE
//    //    else if (png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE) {
//    //        uint8_t* src_address = rows_pointer[y];
//    //        uint8_t* dst_address = (uint8_t*)image->getPixelAddress(0, y);
//    //        unsigned int x;
//
//    //        for (x = 0; x < width; x++)
//    //            *(dst_address++) = *(src_address++);
//    //    }
//    //    png_free(png, rows_pointer[y]);
//    //}
//    //png_free(png, rows_pointer);
//
//    //// Setup the color space.
//    //auto colorSpace = PngFormat::loadColorSpace(png, info);
//    //if (colorSpace)
//    //    fop->setEmbeddedColorProfile();
//    //else { // sRGB is the default PNG color space.
//    //    colorSpace = gfx::ColorSpace::MakeSRGB();
//    //}
//    //if (colorSpace &&
//    //    fop->document()->sprite()->colorSpace()->type() == gfx::ColorSpace::None) {
//    //    fop->document()->sprite()->setColorSpace(colorSpace);
//    //    fop->document()->notifyColorSpaceChanged();
//    //}
//
//    //ASSERT(opts != nullptr);
//    //if (!opts->isEmpty())
//    //    fop->setLoadedFormatOptions(opts);
//
//    return true;
//}
//
//static void report_png_error(png_structp png, png_const_charp error)
//{
//    std::cout << error << std::endl;
//}


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
