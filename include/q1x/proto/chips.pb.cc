// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: chips.proto

#include "chips.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace datasets {
PROTOBUF_CONSTEXPR Line::Line(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.buy_)*/0
  , /*decltype(_impl_.sell_)*/0
  , /*decltype(_impl_.price_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct LineDefaultTypeInternal {
  PROTOBUF_CONSTEXPR LineDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~LineDefaultTypeInternal() {}
  union {
    Line _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 LineDefaultTypeInternal _Line_default_instance_;
PROTOBUF_CONSTEXPR Chips::Chips(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.dist_)*/{}
  , /*decltype(_impl_.date_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ChipsDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ChipsDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ChipsDefaultTypeInternal() {}
  union {
    Chips _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ChipsDefaultTypeInternal _Chips_default_instance_;
PROTOBUF_CONSTEXPR ChipDistribution::ChipDistribution(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.list_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ChipDistributionDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ChipDistributionDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ChipDistributionDefaultTypeInternal() {}
  union {
    ChipDistribution _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ChipDistributionDefaultTypeInternal _ChipDistribution_default_instance_;
}  // namespace datasets
static ::_pb::Metadata file_level_metadata_chips_2eproto[3];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_chips_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_chips_2eproto = nullptr;

const uint32_t TableStruct_chips_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::datasets::Line, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::datasets::Line, _impl_.price_),
  PROTOBUF_FIELD_OFFSET(::datasets::Line, _impl_.buy_),
  PROTOBUF_FIELD_OFFSET(::datasets::Line, _impl_.sell_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::datasets::Chips, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::datasets::Chips, _impl_.date_),
  PROTOBUF_FIELD_OFFSET(::datasets::Chips, _impl_.dist_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::datasets::ChipDistribution, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::datasets::ChipDistribution, _impl_.list_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::datasets::Line)},
  { 9, -1, -1, sizeof(::datasets::Chips)},
  { 17, -1, -1, sizeof(::datasets::ChipDistribution)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::datasets::_Line_default_instance_._instance,
  &::datasets::_Chips_default_instance_._instance,
  &::datasets::_ChipDistribution_default_instance_._instance,
};

const char descriptor_table_protodef_chips_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013chips.proto\022\010datasets\"0\n\004Line\022\r\n\005price"
  "\030\001 \001(\005\022\013\n\003buy\030\002 \001(\001\022\014\n\004sell\030\003 \001(\001\"3\n\005Chi"
  "ps\022\014\n\004date\030\001 \001(\t\022\034\n\004dist\030\002 \003(\0132\016.dataset"
  "s.Line\"1\n\020ChipDistribution\022\035\n\004list\030\001 \003(\013"
  "2\017.datasets.Chipsb\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_chips_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_chips_2eproto = {
    false, false, 185, descriptor_table_protodef_chips_2eproto,
    "chips.proto",
    &descriptor_table_chips_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_chips_2eproto::offsets,
    file_level_metadata_chips_2eproto, file_level_enum_descriptors_chips_2eproto,
    file_level_service_descriptors_chips_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_chips_2eproto_getter() {
  return &descriptor_table_chips_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_chips_2eproto(&descriptor_table_chips_2eproto);
namespace datasets {

// ===================================================================

class Line::_Internal {
 public:
};

Line::Line(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:datasets.Line)
}
Line::Line(const Line& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Line* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.buy_){}
    , decltype(_impl_.sell_){}
    , decltype(_impl_.price_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.buy_, &from._impl_.buy_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.price_) -
    reinterpret_cast<char*>(&_impl_.buy_)) + sizeof(_impl_.price_));
  // @@protoc_insertion_point(copy_constructor:datasets.Line)
}

inline void Line::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.buy_){0}
    , decltype(_impl_.sell_){0}
    , decltype(_impl_.price_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

Line::~Line() {
  // @@protoc_insertion_point(destructor:datasets.Line)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Line::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Line::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Line::Clear() {
// @@protoc_insertion_point(message_clear_start:datasets.Line)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&_impl_.buy_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.price_) -
      reinterpret_cast<char*>(&_impl_.buy_)) + sizeof(_impl_.price_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Line::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 price = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.price_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // double buy = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 17)) {
          _impl_.buy_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      // double sell = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 25)) {
          _impl_.sell_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Line::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:datasets.Line)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 price = 1;
  if (this->_internal_price() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_price(), target);
  }

  // double buy = 2;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_buy = this->_internal_buy();
  uint64_t raw_buy;
  memcpy(&raw_buy, &tmp_buy, sizeof(tmp_buy));
  if (raw_buy != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteDoubleToArray(2, this->_internal_buy(), target);
  }

  // double sell = 3;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_sell = this->_internal_sell();
  uint64_t raw_sell;
  memcpy(&raw_sell, &tmp_sell, sizeof(tmp_sell));
  if (raw_sell != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteDoubleToArray(3, this->_internal_sell(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:datasets.Line)
  return target;
}

size_t Line::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:datasets.Line)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // double buy = 2;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_buy = this->_internal_buy();
  uint64_t raw_buy;
  memcpy(&raw_buy, &tmp_buy, sizeof(tmp_buy));
  if (raw_buy != 0) {
    total_size += 1 + 8;
  }

  // double sell = 3;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_sell = this->_internal_sell();
  uint64_t raw_sell;
  memcpy(&raw_sell, &tmp_sell, sizeof(tmp_sell));
  if (raw_sell != 0) {
    total_size += 1 + 8;
  }

  // int32 price = 1;
  if (this->_internal_price() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_price());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Line::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Line::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Line::GetClassData() const { return &_class_data_; }


void Line::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Line*>(&to_msg);
  auto& from = static_cast<const Line&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:datasets.Line)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_buy = from._internal_buy();
  uint64_t raw_buy;
  memcpy(&raw_buy, &tmp_buy, sizeof(tmp_buy));
  if (raw_buy != 0) {
    _this->_internal_set_buy(from._internal_buy());
  }
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_sell = from._internal_sell();
  uint64_t raw_sell;
  memcpy(&raw_sell, &tmp_sell, sizeof(tmp_sell));
  if (raw_sell != 0) {
    _this->_internal_set_sell(from._internal_sell());
  }
  if (from._internal_price() != 0) {
    _this->_internal_set_price(from._internal_price());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Line::CopyFrom(const Line& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:datasets.Line)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Line::IsInitialized() const {
  return true;
}

void Line::InternalSwap(Line* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Line, _impl_.price_)
      + sizeof(Line::_impl_.price_)
      - PROTOBUF_FIELD_OFFSET(Line, _impl_.buy_)>(
          reinterpret_cast<char*>(&_impl_.buy_),
          reinterpret_cast<char*>(&other->_impl_.buy_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Line::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_chips_2eproto_getter, &descriptor_table_chips_2eproto_once,
      file_level_metadata_chips_2eproto[0]);
}

// ===================================================================

class Chips::_Internal {
 public:
};

Chips::Chips(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:datasets.Chips)
}
Chips::Chips(const Chips& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Chips* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.dist_){from._impl_.dist_}
    , decltype(_impl_.date_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.date_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.date_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_date().empty()) {
    _this->_impl_.date_.Set(from._internal_date(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:datasets.Chips)
}

inline void Chips::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.dist_){arena}
    , decltype(_impl_.date_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.date_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.date_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

Chips::~Chips() {
  // @@protoc_insertion_point(destructor:datasets.Chips)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Chips::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.dist_.~RepeatedPtrField();
  _impl_.date_.Destroy();
}

void Chips::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Chips::Clear() {
// @@protoc_insertion_point(message_clear_start:datasets.Chips)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.dist_.Clear();
  _impl_.date_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Chips::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string date = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_date();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "datasets.Chips.date"));
        } else
          goto handle_unusual;
        continue;
      // repeated .datasets.Line dist = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_dist(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Chips::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:datasets.Chips)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string date = 1;
  if (!this->_internal_date().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_date().data(), static_cast<int>(this->_internal_date().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "datasets.Chips.date");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_date(), target);
  }

  // repeated .datasets.Line dist = 2;
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_dist_size()); i < n; i++) {
    const auto& repfield = this->_internal_dist(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(2, repfield, repfield.GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:datasets.Chips)
  return target;
}

size_t Chips::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:datasets.Chips)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .datasets.Line dist = 2;
  total_size += 1UL * this->_internal_dist_size();
  for (const auto& msg : this->_impl_.dist_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // string date = 1;
  if (!this->_internal_date().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_date());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Chips::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Chips::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Chips::GetClassData() const { return &_class_data_; }


void Chips::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Chips*>(&to_msg);
  auto& from = static_cast<const Chips&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:datasets.Chips)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.dist_.MergeFrom(from._impl_.dist_);
  if (!from._internal_date().empty()) {
    _this->_internal_set_date(from._internal_date());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Chips::CopyFrom(const Chips& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:datasets.Chips)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Chips::IsInitialized() const {
  return true;
}

void Chips::InternalSwap(Chips* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.dist_.InternalSwap(&other->_impl_.dist_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.date_, lhs_arena,
      &other->_impl_.date_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata Chips::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_chips_2eproto_getter, &descriptor_table_chips_2eproto_once,
      file_level_metadata_chips_2eproto[1]);
}

// ===================================================================

class ChipDistribution::_Internal {
 public:
};

ChipDistribution::ChipDistribution(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:datasets.ChipDistribution)
}
ChipDistribution::ChipDistribution(const ChipDistribution& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ChipDistribution* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.list_){from._impl_.list_}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:datasets.ChipDistribution)
}

inline void ChipDistribution::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.list_){arena}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

ChipDistribution::~ChipDistribution() {
  // @@protoc_insertion_point(destructor:datasets.ChipDistribution)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ChipDistribution::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.list_.~RepeatedPtrField();
}

void ChipDistribution::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ChipDistribution::Clear() {
// @@protoc_insertion_point(message_clear_start:datasets.ChipDistribution)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.list_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ChipDistribution::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated .datasets.Chips list = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_list(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* ChipDistribution::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:datasets.ChipDistribution)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .datasets.Chips list = 1;
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_list_size()); i < n; i++) {
    const auto& repfield = this->_internal_list(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(1, repfield, repfield.GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:datasets.ChipDistribution)
  return target;
}

size_t ChipDistribution::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:datasets.ChipDistribution)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .datasets.Chips list = 1;
  total_size += 1UL * this->_internal_list_size();
  for (const auto& msg : this->_impl_.list_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ChipDistribution::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ChipDistribution::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ChipDistribution::GetClassData() const { return &_class_data_; }


void ChipDistribution::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ChipDistribution*>(&to_msg);
  auto& from = static_cast<const ChipDistribution&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:datasets.ChipDistribution)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.list_.MergeFrom(from._impl_.list_);
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ChipDistribution::CopyFrom(const ChipDistribution& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:datasets.ChipDistribution)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ChipDistribution::IsInitialized() const {
  return true;
}

void ChipDistribution::InternalSwap(ChipDistribution* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.list_.InternalSwap(&other->_impl_.list_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ChipDistribution::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_chips_2eproto_getter, &descriptor_table_chips_2eproto_once,
      file_level_metadata_chips_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace datasets
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::datasets::Line*
Arena::CreateMaybeMessage< ::datasets::Line >(Arena* arena) {
  return Arena::CreateMessageInternal< ::datasets::Line >(arena);
}
template<> PROTOBUF_NOINLINE ::datasets::Chips*
Arena::CreateMaybeMessage< ::datasets::Chips >(Arena* arena) {
  return Arena::CreateMessageInternal< ::datasets::Chips >(arena);
}
template<> PROTOBUF_NOINLINE ::datasets::ChipDistribution*
Arena::CreateMaybeMessage< ::datasets::ChipDistribution >(Arena* arena) {
  return Arena::CreateMessageInternal< ::datasets::ChipDistribution >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
