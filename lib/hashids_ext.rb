require "hashids_ext/version"
require "hashids_ext/hashids_ext"

# hashids = HashidsExt.new('salt', 8, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890')
# id_hash = hashids.encode(100.times.to_a)
# ids = hashids.decode(id_hash)
module HashidsExt
  class Hashids
    DEFAULT_ALPHABET = ("abcdefghijklmnopqrstuvwxyz" +
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" +
                        "1234567890").freeze

    def initialize(salt = "", min_hash_length = 0, alphabet = DEFAULT_ALPHABET)
      hashids_initialize(salt, min_hash_length, alphabet)
    end

    def encode(*numbers)
      hashids_encode(numbers)
    end

    def decode(hash)
      return [] if hash.nil? || hash.empty?

      hashids_decode(hash);
    end
  end
end
