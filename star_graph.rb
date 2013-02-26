#!/usr/bin/env ruby

# Parse the apache access log file and display the number of requests per hour
# as a number and a graph

MONTHS = {'Jan' => '01', 'Feb' => '02', 'Mar' => '03', "Apr" => '04', "May" => '05', "Jun" => '06', "Jul" => '07', "Aug" => '08', "Sep" => '09', "Oct" => '10', "Nov" => '11', "Dec" => '12'}

# Convert the time string in the log file "17/Feb/2013:15:28:36 +0000"
# into a string usable as a bucket to collect hourly data.
#
# Why not just DateTime.parse the line? The performance hit for logs
# with more than a million lines is far too great! Even using split
# is more expensive than accessing substrings.
#
# Remember that we will call this function for *each* line in the 
# log files. This is the biggest bottleneck that we can fix.

def make_bucket(line)
  y = line[7..10]
  mo = line[3..5]
  d = line[0..1]
  h = line[12..13]

  "#{y}-#{MONTHS[mo]}-#{d} #{h}:--:--"
end

def stars(max, length)
  x = length / max

  # We add 1 to make sure we display at least 1 star
  y = (100 * x) + 1

  "*" * y
end

data = Hash.new(0.0)

ARGF.each do |line|
  start_of_time = line.index('[')
  end_of_time = line.index(']')

  if start_of_time and end_of_time
    bucket = make_bucket(line[(start_of_time + 1)...end_of_time])

    data[bucket] += 1.0
  end
end

max = data.values.sort.last

data.keys.sort.each do |k|
  puts "%19s [%7d] (%7.2f) %s" % [
    k, 
    data[k], # Number of requests in the hour
    data[k] / 3600.0, # Number of requests per second
    stars(max, data[k])
  ]
end
