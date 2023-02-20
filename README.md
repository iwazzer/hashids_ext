# HashidsExt

C extension of [Hashids](https://github.com/peterhellberg/hashids.rb).

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'hashids_ext'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install hashids_ext

## Usage

```
require 'hashids_ext'
id = 999
hashid = HashidsExt::Hashids.new('this is my salt', 8).encode(id)
decoded = HashidsExt::Hashids.new('this is my salt', 8).decode(hashid)
puts [id, hashid, decoded[0]].join(' -> ')
```

## Performance

```
# Original ruby
result:0.1431s [Hashids: always new instance, repeats:1000]
result:0.0573s [Hashids: same instance, repeats:1000]

# C extension
result:0.0050s [HashidsExt::Hashids: always new instance, repeats:1000]
result:0.0010s [HashidsExt::Hashids: same instance, repeats:1000]
```

## TODO

Add test.

## Development

TODO

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/iwazzer/hashids_ext.
