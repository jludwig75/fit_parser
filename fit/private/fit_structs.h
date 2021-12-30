#pragma

#include <cstdint>


struct __attribute__((__packed__)) FitFileHeaderLegacy
{
    uint8_t header_size;
    uint8_t protocol_version;
    uint16_t profile_version;
    uint32_t data_size;
    char data_type[4];
};
static_assert(sizeof(FitFileHeaderLegacy) == 12);

struct __attribute__((__packed__)) FitFileHeader : public FitFileHeaderLegacy
{
    uint16_t crc;
};
static_assert(sizeof(FitFileHeader) == 14);


struct __attribute__((__packed__)) FitRecordHeader
{
    uint8_t local_message_type : 4;
    uint8_t reserved : 1;
    uint8_t message_type_specific : 1;
    uint8_t message_type : 1;
    uint8_t normal_header : 1;
};
static_assert(sizeof(FitRecordHeader) == 1);

#define MESSAGE_TYPE_DATA       0
#define MESSAGE_TYPE_DEFINITION 1


struct __attribute__((__packed__)) FitRecordDefinitionHeader
{
    uint8_t reserved;
    uint8_t architecture;
    uint16_t global_message_number;
    uint8_t fields;

};
static_assert(sizeof(FitRecordDefinitionHeader) == 5);

#define DEFINITION_ARCHITECTURE_LITTLE_ENDIAN   0
#define DEFINITION_ARCHITECTURE_BIG_ENDIAN      1


struct __attribute__((__packed__)) FitFieldDefinition
{
    uint8_t field_definiton_number;
    uint8_t size;
    uint8_t base_type;
};
static_assert(sizeof(FitFieldDefinition) == 3);


struct __attribute__((__packed__)) FitDeveloperFieldDefinition
{
    uint8_t field_number;
    uint8_t size;
    uint8_t developer_data_index;
};
static_assert(sizeof(FitDeveloperFieldDefinition) == 3);
