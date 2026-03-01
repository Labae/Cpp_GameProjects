//
// Created by Labae on 1/12/26.
//

#ifndef RESOURCES_HPP
#define RESOURCES_HPP

namespace Tetris::Resources
{
    struct Resource
    {
        const char* Id;
        const char* Path;
    };

    namespace Fonts
    {
        constexpr Resource Main{"main", "assets/fonts/NotoSans_Black.ttf"};
    }

    // 나중에 추가
    // namespace Sounds
    // {
    // }

    // namespace Music
    // {
    // }
} // namespace Tetris::Resources

#endif //RESOURCES_HPP
