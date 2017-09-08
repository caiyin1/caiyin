local function mergeData (data)
  	local str = table.concat(data)
  	for Temp in string.gmatch(str, '([^ ]+)') do
		print(string.format("    %s\n", Temp))
	end
end

--读取txt文件
local function openFile(filePath)
	local file = io.open(filePath, "r")
	if file == nil then 
		print("open error")
	end
	local data = {}
	for line in file:lines() do
		table.insert(data, line)
	end
	return data
end

data = openFile("D:\\xiaoshuo.txt")
mergeData(data)
