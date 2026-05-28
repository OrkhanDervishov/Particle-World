// #include "assets.h"


// int load_asset(AssetImage* image, const char* path){
//     return load_png(&image->image, path);
// }

// int asset_to_array_image(ArrayImage* arr_image, AssetImage image, int offset_x, int offset_y){
//     int res = copy_image(&arr_image->image, image.image);
//     if(!res) return res;

//     arr_image->offset_x = offset_x;
//     arr_image->offset_y = offset_y;
//     arr_image->cols = image.image.width/offset_x;
//     arr_image->rows = image.image.height/offset_y;
//     return 0;
// }

// Image array_image_get_frame(ArrayImage* arr_image, int frame_index){
//     Image image = {
//         .width = arr_image->offset_x, 
//         .height = arr_image->offset_y,
//         .buffer = NULL
//     };
//     if(frame_index >= arr_image->offset_x*arr_image->offset_y){
//         image.width = 0;
//         image.height = 0;
//         return image;
//     }

//     int x = (frame_index % arr_image->cols) * arr_image->offset_x;
//     int y = (frame_index / arr_image->rows) * arr_image->offset_y;
//     int index = arr_image->image.width*y + x;

//     image.buffer = arr_image->image.buffer + index;
//     return image;
// }

// int create_animation(Animation* anim){
    
//     return 0;
// }

// void array_image_to_anim(Animation* anim, ArrayImage* arr_image){
//     anim->frame_images = arr_image;
// }

// void anim_add_order(Animation* anim, int* order){
    
// }
// void anim_add_delays(Animation* anim, float* delays){

// }
// Image anim_get_frame(Animation* anim, int frame_index){
    
// }