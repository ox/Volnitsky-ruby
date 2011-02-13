require "./volnitsky"
include Volnitsky

S = ""

File.open("test_text.txt","r") do |f|
  while line = f.gets
    S << line
  end
end

puts "="*80, "Testing Volnitsky Substring Search Algorithm","="*80

SS = "The crew managed to escape onto the ice and built a makeshift airstrip using only a few spades, ice shovels and two crowbars. They had to rebuild the airstrip thirteen times, until they were rescued in April of the same year and flown to the village of [[Vankarem]] on the coast of the sea. From there, some of the"

sizes = [3,4,5,6,7,8]# ,9, 11, 13, 16, 20, 30,  40, 60, 120, (SS.length) - 1]

puts "S.length = #{S.length}, SS.length = #{SS.length}"


sizes.each do |size|
  found_at = search(S,SS[0...size])
  k = Time.now
  puts "size: #{size}, found_at: #{found_at}, string_at_pos: #{S[found_at...found_at+size]}, Took #{(Time.now.usec - k.usec)} usec"
end

puts "="*80, "Testing #scan","="*80

sizes.each do |size|
  found_at = S.scan(SS[0...size])
  k = Time.now
  puts "size: #{size}, found: #{found_at}, Took #{(Time.now.usec - k.usec)} usec"
end

puts "="*80, "Testing #sub","="*80
sizes.each do |size|
  found_at = S.sub(SS[0...size], "")
  k = Time.now
  puts "size: #{size}, Took #{(Time.now.usec - k.usec)} usec"
end