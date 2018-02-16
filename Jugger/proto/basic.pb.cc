// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: basic.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "basic.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace BasicProtobuf {

namespace {

const ::google::protobuf::Descriptor* ConnectionResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ConnectionResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* ConnectionResponse_ResponseCode_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_basic_2eproto() {
  protobuf_AddDesc_basic_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "basic.proto");
  GOOGLE_CHECK(file != NULL);
  ConnectionResponse_descriptor_ = file->message_type(0);
  static const int ConnectionResponse_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectionResponse, responsecode_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectionResponse, sessionid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectionResponse, message_),
  };
  ConnectionResponse_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      ConnectionResponse_descriptor_,
      ConnectionResponse::default_instance_,
      ConnectionResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectionResponse, _has_bits_[0]),
      -1,
      -1,
      sizeof(ConnectionResponse),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectionResponse, _internal_metadata_),
      -1);
  ConnectionResponse_ResponseCode_descriptor_ = ConnectionResponse_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_basic_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      ConnectionResponse_descriptor_, &ConnectionResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_basic_2eproto() {
  delete ConnectionResponse::default_instance_;
  delete ConnectionResponse_reflection_;
}

void protobuf_AddDesc_basic_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013basic.proto\022\rBasicProtobuf\"\230\002\n\022Connect"
    "ionResponse\022D\n\014responseCode\030\001 \002(\0162..Basi"
    "cProtobuf.ConnectionResponse.ResponseCod"
    "e\022\021\n\tsessionId\030\002 \002(\t\022\017\n\007message\030\003 \001(\t\"\227\001"
    "\n\014ResponseCode\022\007\n\002OK\020\310\001\022\031\n\024BadMessageFor"
    "matting\020\220\003\022\023\n\016UnknownService\020\224\003\022\030\n\023NotSu"
    "pportedVersion\020\225\003\022\014\n\007Timeout\020\230\003\022\022\n\rProto"
    "colError\020\232\003\022\022\n\rInternalError\020\364\003", 311);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "basic.proto", &protobuf_RegisterTypes);
  ConnectionResponse::default_instance_ = new ConnectionResponse();
  ConnectionResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_basic_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_basic_2eproto {
  StaticDescriptorInitializer_basic_2eproto() {
    protobuf_AddDesc_basic_2eproto();
  }
} static_descriptor_initializer_basic_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD;
static void MergeFromFail(int line) {
  GOOGLE_CHECK(false) << __FILE__ << ":" << line;
}

}  // namespace


// ===================================================================

const ::google::protobuf::EnumDescriptor* ConnectionResponse_ResponseCode_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ConnectionResponse_ResponseCode_descriptor_;
}
bool ConnectionResponse_ResponseCode_IsValid(int value) {
  switch(value) {
    case 200:
    case 400:
    case 404:
    case 405:
    case 408:
    case 410:
    case 500:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const ConnectionResponse_ResponseCode ConnectionResponse::OK;
const ConnectionResponse_ResponseCode ConnectionResponse::BadMessageFormatting;
const ConnectionResponse_ResponseCode ConnectionResponse::UnknownService;
const ConnectionResponse_ResponseCode ConnectionResponse::NotSupportedVersion;
const ConnectionResponse_ResponseCode ConnectionResponse::Timeout;
const ConnectionResponse_ResponseCode ConnectionResponse::ProtocolError;
const ConnectionResponse_ResponseCode ConnectionResponse::InternalError;
const ConnectionResponse_ResponseCode ConnectionResponse::ResponseCode_MIN;
const ConnectionResponse_ResponseCode ConnectionResponse::ResponseCode_MAX;
const int ConnectionResponse::ResponseCode_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int ConnectionResponse::kResponseCodeFieldNumber;
const int ConnectionResponse::kSessionIdFieldNumber;
const int ConnectionResponse::kMessageFieldNumber;
#endif  // !_MSC_VER

ConnectionResponse::ConnectionResponse()
  : ::google::protobuf::Message() , _internal_metadata_(NULL)  {
  SharedCtor();
  // @@protoc_insertion_point(constructor:BasicProtobuf.ConnectionResponse)
}

void ConnectionResponse::InitAsDefaultInstance() {
}

ConnectionResponse::ConnectionResponse(const ConnectionResponse& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:BasicProtobuf.ConnectionResponse)
}

void ConnectionResponse::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  responsecode_ = 200;
  sessionid_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ConnectionResponse::~ConnectionResponse() {
  // @@protoc_insertion_point(destructor:BasicProtobuf.ConnectionResponse)
  SharedDtor();
}

void ConnectionResponse::SharedDtor() {
  sessionid_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  message_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != default_instance_) {
  }
}

void ConnectionResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ConnectionResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ConnectionResponse_descriptor_;
}

const ConnectionResponse& ConnectionResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_basic_2eproto();
  return *default_instance_;
}

ConnectionResponse* ConnectionResponse::default_instance_ = NULL;

ConnectionResponse* ConnectionResponse::New(::google::protobuf::Arena* arena) const {
  ConnectionResponse* n = new ConnectionResponse;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void ConnectionResponse::Clear() {
  if (_has_bits_[0 / 32] & 7) {
    responsecode_ = 200;
    if (has_sessionid()) {
      sessionid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    if (has_message()) {
      message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool ConnectionResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:BasicProtobuf.ConnectionResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .BasicProtobuf.ConnectionResponse.ResponseCode responseCode = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::BasicProtobuf::ConnectionResponse_ResponseCode_IsValid(value)) {
            set_responsecode(static_cast< ::BasicProtobuf::ConnectionResponse_ResponseCode >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_sessionId;
        break;
      }

      // required string sessionId = 2;
      case 2: {
        if (tag == 18) {
         parse_sessionId:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_sessionid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->sessionid().data(), this->sessionid().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "BasicProtobuf.ConnectionResponse.sessionId");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_message;
        break;
      }

      // optional string message = 3;
      case 3: {
        if (tag == 26) {
         parse_message:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->message().data(), this->message().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "BasicProtobuf.ConnectionResponse.message");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:BasicProtobuf.ConnectionResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:BasicProtobuf.ConnectionResponse)
  return false;
#undef DO_
}

void ConnectionResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:BasicProtobuf.ConnectionResponse)
  // required .BasicProtobuf.ConnectionResponse.ResponseCode responseCode = 1;
  if (has_responsecode()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->responsecode(), output);
  }

  // required string sessionId = 2;
  if (has_sessionid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->sessionid().data(), this->sessionid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "BasicProtobuf.ConnectionResponse.sessionId");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->sessionid(), output);
  }

  // optional string message = 3;
  if (has_message()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "BasicProtobuf.ConnectionResponse.message");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->message(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:BasicProtobuf.ConnectionResponse)
}

::google::protobuf::uint8* ConnectionResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:BasicProtobuf.ConnectionResponse)
  // required .BasicProtobuf.ConnectionResponse.ResponseCode responseCode = 1;
  if (has_responsecode()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->responsecode(), target);
  }

  // required string sessionId = 2;
  if (has_sessionid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->sessionid().data(), this->sessionid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "BasicProtobuf.ConnectionResponse.sessionId");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->sessionid(), target);
  }

  // optional string message = 3;
  if (has_message()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "BasicProtobuf.ConnectionResponse.message");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->message(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:BasicProtobuf.ConnectionResponse)
  return target;
}

int ConnectionResponse::RequiredFieldsByteSizeFallback() const {
  int total_size = 0;

  if (has_responsecode()) {
    // required .BasicProtobuf.ConnectionResponse.ResponseCode responseCode = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->responsecode());
  }

  if (has_sessionid()) {
    // required string sessionId = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->sessionid());
  }

  return total_size;
}
int ConnectionResponse::ByteSize() const {
  int total_size = 0;

  if (((_has_bits_[0] & 0x00000003) ^ 0x00000003) == 0) {  // All required fields are present.
    // required .BasicProtobuf.ConnectionResponse.ResponseCode responseCode = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->responsecode());

    // required string sessionId = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->sessionid());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  // optional string message = 3;
  if (has_message()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->message());
  }

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ConnectionResponse::MergeFrom(const ::google::protobuf::Message& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const ConnectionResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ConnectionResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ConnectionResponse::MergeFrom(const ConnectionResponse& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_responsecode()) {
      set_responsecode(from.responsecode());
    }
    if (from.has_sessionid()) {
      set_has_sessionid();
      sessionid_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.sessionid_);
    }
    if (from.has_message()) {
      set_has_message();
      message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void ConnectionResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ConnectionResponse::CopyFrom(const ConnectionResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConnectionResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void ConnectionResponse::Swap(ConnectionResponse* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ConnectionResponse::InternalSwap(ConnectionResponse* other) {
  std::swap(responsecode_, other->responsecode_);
  sessionid_.Swap(&other->sessionid_);
  message_.Swap(&other->message_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata ConnectionResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ConnectionResponse_descriptor_;
  metadata.reflection = ConnectionResponse_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// ConnectionResponse

// required .BasicProtobuf.ConnectionResponse.ResponseCode responseCode = 1;
 bool ConnectionResponse::has_responsecode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
 void ConnectionResponse::set_has_responsecode() {
  _has_bits_[0] |= 0x00000001u;
}
 void ConnectionResponse::clear_has_responsecode() {
  _has_bits_[0] &= ~0x00000001u;
}
 void ConnectionResponse::clear_responsecode() {
  responsecode_ = 200;
  clear_has_responsecode();
}
 ::BasicProtobuf::ConnectionResponse_ResponseCode ConnectionResponse::responsecode() const {
  // @@protoc_insertion_point(field_get:BasicProtobuf.ConnectionResponse.responseCode)
  return static_cast< ::BasicProtobuf::ConnectionResponse_ResponseCode >(responsecode_);
}
 void ConnectionResponse::set_responsecode(::BasicProtobuf::ConnectionResponse_ResponseCode value) {
  assert(::BasicProtobuf::ConnectionResponse_ResponseCode_IsValid(value));
  set_has_responsecode();
  responsecode_ = value;
  // @@protoc_insertion_point(field_set:BasicProtobuf.ConnectionResponse.responseCode)
}

// required string sessionId = 2;
 bool ConnectionResponse::has_sessionid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
 void ConnectionResponse::set_has_sessionid() {
  _has_bits_[0] |= 0x00000002u;
}
 void ConnectionResponse::clear_has_sessionid() {
  _has_bits_[0] &= ~0x00000002u;
}
 void ConnectionResponse::clear_sessionid() {
  sessionid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_sessionid();
}
 const ::std::string& ConnectionResponse::sessionid() const {
  // @@protoc_insertion_point(field_get:BasicProtobuf.ConnectionResponse.sessionId)
  return sessionid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void ConnectionResponse::set_sessionid(const ::std::string& value) {
  set_has_sessionid();
  sessionid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:BasicProtobuf.ConnectionResponse.sessionId)
}
 void ConnectionResponse::set_sessionid(const char* value) {
  set_has_sessionid();
  sessionid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:BasicProtobuf.ConnectionResponse.sessionId)
}
 void ConnectionResponse::set_sessionid(const char* value, size_t size) {
  set_has_sessionid();
  sessionid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:BasicProtobuf.ConnectionResponse.sessionId)
}
 ::std::string* ConnectionResponse::mutable_sessionid() {
  set_has_sessionid();
  // @@protoc_insertion_point(field_mutable:BasicProtobuf.ConnectionResponse.sessionId)
  return sessionid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* ConnectionResponse::release_sessionid() {
  clear_has_sessionid();
  return sessionid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void ConnectionResponse::set_allocated_sessionid(::std::string* sessionid) {
  if (sessionid != NULL) {
    set_has_sessionid();
  } else {
    clear_has_sessionid();
  }
  sessionid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sessionid);
  // @@protoc_insertion_point(field_set_allocated:BasicProtobuf.ConnectionResponse.sessionId)
}

// optional string message = 3;
 bool ConnectionResponse::has_message() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
 void ConnectionResponse::set_has_message() {
  _has_bits_[0] |= 0x00000004u;
}
 void ConnectionResponse::clear_has_message() {
  _has_bits_[0] &= ~0x00000004u;
}
 void ConnectionResponse::clear_message() {
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_message();
}
 const ::std::string& ConnectionResponse::message() const {
  // @@protoc_insertion_point(field_get:BasicProtobuf.ConnectionResponse.message)
  return message_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void ConnectionResponse::set_message(const ::std::string& value) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:BasicProtobuf.ConnectionResponse.message)
}
 void ConnectionResponse::set_message(const char* value) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:BasicProtobuf.ConnectionResponse.message)
}
 void ConnectionResponse::set_message(const char* value, size_t size) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:BasicProtobuf.ConnectionResponse.message)
}
 ::std::string* ConnectionResponse::mutable_message() {
  set_has_message();
  // @@protoc_insertion_point(field_mutable:BasicProtobuf.ConnectionResponse.message)
  return message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* ConnectionResponse::release_message() {
  clear_has_message();
  return message_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void ConnectionResponse::set_allocated_message(::std::string* message) {
  if (message != NULL) {
    set_has_message();
  } else {
    clear_has_message();
  }
  message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:BasicProtobuf.ConnectionResponse.message)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace BasicProtobuf

// @@protoc_insertion_point(global_scope)