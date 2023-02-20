#include "hashids.h"
#include "hashids_ext.h"

VALUE rb_mHashidsExt;
VALUE rb_cHashidsExtHashids;
VALUE rb_cHashidsStruct;


void
Init_hashids_ext(void)
{
  rb_mHashidsExt = rb_define_module("HashidsExt");
  rb_cHashidsExtHashids = rb_define_class_under(rb_mHashidsExt, "Hashids", rb_cObject);
  rb_cHashidsStruct = rb_define_class_under(rb_mHashidsExt, "HashidsStruct", rb_cObject);

  // Add interface for Hashids.c
  rb_define_private_method(rb_cHashidsExtHashids, "hashids_initialize", _hashids_initialize, -1);
  rb_define_private_method(rb_cHashidsExtHashids, "hashids_encode", _hashids_encode, -1);
  rb_define_private_method(rb_cHashidsExtHashids, "hashids_decode", _hashids_decode, -1);
}

void
_hashids_ext_free(hashids_t* hashids_struct)
{
  hashids_free(hashids_struct);
}

/*
 * C実装で初期化した hashids_t 構造体をWrapしたインスタンスを返す
 */
VALUE
_hashids_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_salt = argv[0];
    VALUE rb_min_hash_length = argv[1];
    VALUE rb_alphabet = argv[2];

    hashids_t* hashids = hashids_init3(StringValueCStr(rb_salt), NUM2INT(rb_min_hash_length), StringValueCStr(rb_alphabet));
    if (hashids == NULL) {
      rb_raise(rb_eRuntimeError, "hashids_init3 failed");
    }
    VALUE rb_hashids_struct = Data_Wrap_Struct(rb_cHashidsStruct, 0, _hashids_ext_free, hashids);

    rb_ivar_set(self, rb_intern("@hashids_struct"), rb_hashids_struct);

    return Qnil; // 戻り値を使わないので便宜的
}

VALUE
_hashids_encode(int argc, VALUE *argv, VALUE self)
{
  Check_Type(argv[0], T_ARRAY);
  VALUE rb_numbers = argv[0];
  int numbers_len = RARRAY_LENINT(rb_numbers);

  unsigned long long* numbers = (unsigned long long*)calloc(numbers_len, sizeof(unsigned long long));
  if (numbers == NULL) {
    rb_raise(rb_eRuntimeError, "Cannot allocate memory for numbers buffer");
  }
  for (int i=0; i<numbers_len; i++) {
    VALUE number = rb_ary_entry(rb_numbers, i);
    if (TYPE(number) == T_FIXNUM || TYPE(number) == T_BIGNUM) {
      numbers[i] = NUM2ULL(number);
    }
  }

  VALUE rb_hashids_struct = rb_ivar_get(self, rb_intern("@hashids_struct"));
  hashids_t* hashids;
  Data_Get_Struct(rb_hashids_struct, hashids_t, hashids);

  size_t encoded_len = hashids_estimate_encoded_size(hashids, argc, numbers);
  char* encoded = (char*)calloc(encoded_len, sizeof(char));
  if (encoded == NULL) {
    rb_raise(rb_eRuntimeError, "Cannot allocate memory for encoded string buffer");
  }
  hashids_encode(hashids, encoded, numbers_len, numbers);

  VALUE rb_encoded = rb_str_new_cstr(encoded);
  free(encoded);
  free(numbers);

  return rb_encoded;
}

VALUE
_hashids_decode(int argc, VALUE *argv, VALUE self)
{
  Check_Type(argv[0], T_STRING);
  VALUE rb_encoded = argv[0];

  VALUE rb_hashids_struct = rb_ivar_get(self, rb_intern("@hashids_struct"));
  hashids_t* hashids;
  Data_Get_Struct(rb_hashids_struct, hashids_t, hashids);

  const char* encoded = StringValueCStr(rb_encoded);

  size_t numbers_count = hashids_numbers_count(hashids, encoded);
  if (numbers_count == 0) {
    switch (hashids_errno) {
      case HASHIDS_ERROR_INVALID_HASH:
        rb_raise(rb_eRuntimeError, "Hashids: Invalid hash");
      default:
        rb_raise(rb_eRuntimeError, "Hashids: Unknown error");
    }
  }
  unsigned long long* numbers = (unsigned long long*)calloc(numbers_count, sizeof(unsigned long long));
  if (numbers == NULL) {
    rb_raise(rb_eRuntimeError, "Cannot allocate memory for numbers buffer");
  }

  if (hashids_decode(hashids, encoded, numbers, numbers_count) != numbers_count) {
    rb_raise(rb_eRuntimeError, "Hashids: Could not decode numbers properly");
  }

  VALUE rb_ull_ary = rb_ary_new2(numbers_count);
  for (long i=0; i<numbers_count; i++) {
    rb_ary_store(rb_ull_ary, i, ULL2NUM(numbers[i]));
  }
  free(numbers);

  return rb_ull_ary;
}
