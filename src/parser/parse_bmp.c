#include "parse_bmp.h"

/**
 * @brief Display the content of the BMP header and info header
 * @param header pointer to the BMP header
 * @param info pointer to the BMP info header
*/
FT_INLINE void display_bmp_hdr_data(bmp_hdr *header, bmp_info_hdr *info) {
    /* display size of structure */    
    ft_printf_fd(1, "Size of BMP Header: %d\n", BMP_HDR_SIZE);
    ft_printf_fd(1, "Size of BMP Info Header: %d\n", BMP_INFO_HDR_SIZE);
    /* display header struct field  */
    ft_printf_fd(1, "Type: %c%c\n", (char)header->type, (char)(header->type >> 8));
    ft_printf_fd(1, "Size: %d\n", header->size);
    ft_printf_fd(1, "Reserved1: %d\n", header->reserved1);
    ft_printf_fd(1, "Reserved2: %d\n", header->reserved2);
    ft_printf_fd(1, "Offset: %d\n", header->offset);
    /* display header info field */
    ft_printf_fd(1, "Size: %d\n", info->size);
    ft_printf_fd(1, "Width: %d\n", info->width);
    ft_printf_fd(1, "Height: %d\n", info->height);
    ft_printf_fd(1, "Planes: %d\n", info->planes);
    ft_printf_fd(1, "Bits: %d\n", info->bits);
    ft_printf_fd(1, "Compression: %d\n", info->compression);
    ft_printf_fd(1, "ImageSize: %d\n", info->imagesize);
    ft_printf_fd(1, "XResolution: %d\n", info->xresolution);
    ft_printf_fd(1, "YResolution: %d\n", info->yresolution);
    ft_printf_fd(1, "NColours: %d\n", info->ncolours);
    ft_printf_fd(1, "ImportantColours: %d\n", info->important_colours);
}

/**
 * @brief Load a file into memory using mmap
 * @param path path to the file to read
 * @param file_size pointer to store the file size
 * @return pointer to the file in memory 
*/
FT_INLINE u8 *load_mmap_file(char *path, u64 *file_size)
{
    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        ft_printf_fd(2, "Failed to open file %s\n", path);
        return (NULL);
    }

    struct stat st;

    ft_bzero(&st, sizeof(st));
    fstat(fd, &st);
    *file_size = st.st_size;

    if (*file_size <= BMP_HDR_TOTAL_SIZE) {
        close(fd);
        ft_printf_fd(2, "File %s is no enought large to contain BMP header %u\n", path, *file_size);
        return (NULL);
    }

    void *map = mmap(0, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (map == MAP_FAILED) {
        close(fd);
        ft_printf_fd(2, "Failed to open file %s\n", path);
        return (NULL);
    }
    return (map);
}

u8 *parse_bmp_file(char* path, int *width, int *height, int *type)
{
    u64 file_size = 0;
    u8 *file = load_mmap_file(path, &file_size);

    if (file == NULL) {
        ft_printf_fd(2, "mmap failled to load file %s\n", path);
        return (NULL);
    }

    bmp_hdr         header;
    bmp_info_hdr    info;

    ft_memcpy(&header, file, BMP_HDR_SIZE);
    ft_memcpy(&info, file + BMP_HDR_SIZE, BMP_INFO_HDR_SIZE);


    int data_size = 0;

    if (info.bits == 24) {
        *type = 3;
        data_size = RGB_SIZE;
    } else if (info.bits == 32) {
        *type = 4;
        data_size = RGBA_SIZE;
    }
    *width = info.width;
    *height = info.height;
    

    u8 *end_ptr = file + file_size;
    u8 *estimate_end = file + header.offset + (info.width * info.height * data_size);
    if (end_ptr != estimate_end) {
        ft_printf_fd(2, "Invalid file size, estimated end ptr %p, real end %p\n", (void *)estimate_end, (void *)end_ptr);
        return (NULL);
    } 

    u8 *image = malloc(info.width * info.height * data_size);
    if (!image) {
        return (NULL);
    }
    u8 *read_ptr = file + header.offset;
    for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.width; j++) {
            u8 *pixel = image + (i * info.width + j) * data_size;
            pixel[0] = read_ptr[2]; // B
            pixel[1] = read_ptr[1]; // G
            pixel[2] = read_ptr[0]; // R
            if (data_size == 4) {
                pixel[3] = read_ptr[3]; // A
            }
            read_ptr += data_size;
        }
    }
    return (image);
}