require 'rubygems'
require 'nokogiri'

def parse(file)
    doc = Nokogiri::HTML(File.open(file)) do |config|
      config.noent.nonet
    end

    test = TestPair.new
    puts test.a
    puts test.b

    a = doc.xpath("(//h1)[1]")
    return a.to_a
end
