//
// Created by Benjamin Watson on 9/9/23.
//

#include "bundle_reader.h"
#include "memory_utils.h"
#include "bundle.h"

 Bundle BundleReader::ReadBundle(pid_t pid) {

    dyld_all_image_infos *infos = MemoryUtils::GetDyldImageInfo(pid);
    bool found = false;
    uint8_t *section_data = nullptr;
    uint64_t section_data_size;

    for (int i = 0; i < infos->infoArrayCount; ++i) {
        if (FindAndLoadSection((mach_header_64 *) infos->infoArray[i].imageLoadAddress,
                               reinterpret_cast<uint8_t **>(&section_data),
                               &section_data_size)) {
            found = true;
            break;
        }
    }
    if(found) {
        return {section_data, section_data_size};
    }
     return {nullptr, 0};
 }

bool BundleReader::FindAndLoadSection(mach_header_64 *header, uint8_t **section_data, uint64_t *section_data_size) {
    // If we have a 64-bit magic value create the start of the load commands
    if (header->magic == MH_MAGIC_64) {
        const load_command *cmd = (load_command *) ((uint8_t *) header + sizeof(mach_header_64));
        for (uint32_t i = 0; i < header->ncmds; ++i) {
            if (cmd->cmd == LC_SEGMENT_64) {
                const segment_command_64 *segment = (segment_command_64 *) cmd;
                if (strcmp(segment->segname, "__TEXT") == 0) {
                    // If we are in the TEXT segment, then calculate the slide
                    uint64_t slide = ((uint64_t) header) - segment->vmaddr;
                    const section_64 *sections_start = (section_64 *) ((uint8_t *) segment +
                                                                       sizeof(struct segment_command_64));
                    const section_64 *sections_end = &sections_start[segment->nsects];
                    for (const section_64 *section = sections_start; section < sections_end; ++section) {
                        // We have our target section
                        if (strcmp(section->sectname, "__sc") == 0) {
                            // Will overlap with the header
                            auto *segment_content_start = reinterpret_cast<uint8_t *>(segment->vmaddr + slide);
                            auto *section_content_start = reinterpret_cast<uint8_t *>(segment_content_start +
                                                                                      (section->addr -
                                                                                       segment->vmaddr));
                            // Assign the pointer to the section data
                            *section_data = section_content_start;
                            *section_data_size = section->size;
                            return true;
                        }
                    }
                }
            }
            // Push into next load command
            const load_command *next = (load_command *) ((char *) cmd + cmd->cmdsize);
            cmd = next;
        }
    }
    return false;
}

