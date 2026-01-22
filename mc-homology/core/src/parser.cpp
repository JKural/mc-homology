#include "../include/core/parser.h"

#include <cstring>

extern "C" {
#include <chunkParser.h>
#include <regionParser.h>
}

#include "core/cubical_complex_3d.h"

namespace {

std::pair<int, int> get_lower_chunk_coords(int block_x, int block_z) {
    return {block_x >> 4, block_z >> 4};
}

std::pair<int, int> get_upper_chunk_coords(int block_x, int block_z) {
    return {(block_x >> 4) + 1, (block_z >> 4) + 1};
}

} // namespace

namespace core {

MinecraftSavefileParser::~MinecraftSavefileParser() = default;

std::unique_ptr<Complex> MinecraftSavefileParser_mcSavefileParsers::parse(
    std::filesystem::path const& path,
    MinecraftCoordinates lower_corner,
    MinecraftCoordinates upper_corner
) {
    CubicalComplex3D complex {};
    auto const [lower_chunk_x, lower_chunk_z] =
        get_lower_chunk_coords(lower_corner.x, lower_corner.z);
    auto const [upper_chunk_x, upper_chunk_z] =
        get_upper_chunk_coords(upper_corner.x, upper_corner.z);
    for (int chunk_x = lower_chunk_x; chunk_x < upper_chunk_x; ++chunk_x) {
        for (int chunk_z = lower_chunk_z; chunk_z < upper_chunk_z; ++chunk_z) {
            // we const cast, because the original C API doesn't accept const char*
            // it's fine, because extractChunk doesnt modify the name
            auto chunk =
                extractChunk(const_cast<char*>(path.c_str()), chunk_x, chunk_z);
            std::vector<section> sections(maxSections);
            auto count =
                getSections(chunk.data, chunk.byteLength, sections.data());
            sections.resize(count);
            for (auto const& section : sections) {
                int out_len = 0;
                unsigned* block_states = getBlockStates(section, &out_len);
                for (int y = 0; y < 16; ++y) {
                    // out of range
                    if (!(lower_corner.y <= 16 * section.y + y
                          && 16 * section.y + y < upper_corner.y)) {
                        continue;
                    }
                    for (int x = 0; x < 16; ++x) {
                        // out of range
                        if (!(lower_corner.x <= 16 * chunk_x + x
                              && 16 * chunk_x + x < upper_corner.x)) {
                            continue;
                        }
                        for (int z = 0; z < 16; ++z) {
                            // out of range
                            if (!(lower_corner.z <= 16 * chunk_z + z
                                  && 16 * chunk_z + z < upper_corner.z)) {
                                continue;
                            }
                            auto block =
                                createBlock(x, y, z, block_states, section);
                            if (strcmp(block.type, mcAir) != 0) {
                                complex.add_cube(
                                    16 * chunk_x + x,
                                    16 * section.y + y,
                                    16 * chunk_z + z
                                );
                            }
                        }
                    }
                }
                free(block_states);
            }
            // free sections frees inner data of sections and not the
            // pointer itself, so we are safe
            freeSections(sections.data(), sections.size());
            free(chunk.data);
        }
    }
    return std::make_unique<CubicalComplex3D>(std::move(complex));
}

} // namespace core
