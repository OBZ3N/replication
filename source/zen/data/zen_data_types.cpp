#include "zen/data/zen_data_element.h"
#include "zen/data/zen_data_component.h"
#include "zen/data/zen_data_boolean.h"
#include "zen/data/zen_data_float_ranged.h"
#include "zen/data/zen_data_integer_ranged.h"
#include "zen/data/zen_data_string.h"
#include "zen/data/zen_data_raw.h"
#include "zen/data/zen_data_vector.h"

IMPLEMENT_RTTI_TYPE_ID(zen::data::Element, zen::rtti::Base);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Component, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Boolean, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::String,  zen::data::Element);

IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<bool>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<float>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<double>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<uint8_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<uint16_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<uint32_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<uint64_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<int8_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<int16_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<int32_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Raw<int64_t>, zen::data::Element);

IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<uint8_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<uint16_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<uint32_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<uint64_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<int8_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<int16_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<int32_t>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::IntegerRanged<int64_t>, zen::data::Element);

IMPLEMENT_RTTI_TYPE_ID(zen::data::FloatRanged<float>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::FloatRanged<double>, zen::data::Element);


IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Component>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Boolean>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::String>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<bool>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<uint8_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<uint16_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<uint32_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<uint64_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<int8_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<int16_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<int32_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<int64_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<float>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::Raw<double>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<uint8_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<uint16_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<uint32_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<uint64_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<int8_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<int16_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<int32_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::IntegerRanged<int64_t>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::FloatRanged<float>>, zen::data::Element);
IMPLEMENT_RTTI_TYPE_ID(zen::data::Vector<zen::data::FloatRanged<double>>, zen::data::Element);

