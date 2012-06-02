require "test/unit"
require "stacktrace"
require "benchmark"

class Object
  def metaclass; class << self; self; end; end
end

class TestStacktrace < Test::Unit::TestCase
  def test_we_get_an_array
    assert_equal Array, stacktrace.class
  end

  def test_we_get_items_in_our_array
    assert stacktrace.length > 0
  end

  def self.test_singleton
    stacktrace
  end

  def test_singleton_stacktrace
    frame = TestStacktrace.test_singleton[0]
    assert_equal frame[:klass], TestStacktrace.metaclass 
    assert_equal frame[:method], "test_singleton" 
  end
  
  def test_thread_stacktrace
    frame = Thread.current.stacktrace[0]
    assert_equal frame[:klass], TestStacktrace
    assert_equal frame[:method], "test_thread_stacktrace"
  end

  # stacktrace is MUCH faster
  #def test_demo_bench
  #  Benchmark.bm(7) do |b|
  #    b.report("caller: ") { 100000.times { caller } }
  #    b.report("stacktrace: ") { 100000.times { stacktrace } }
  #  end
  #end

end
