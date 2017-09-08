-- 输出head信息
local function putHead (data, file)
	filename = io.open(file, "w+")
	local str = table.concat(data[1], ',')
	filename:write(str)
	filename:write('\n')
	filename:close()
end
 
--单独输出相同属性的信息
-- 1. 数据列表  2. 文件位置  3. 头消息  4. 具体属性
local function gemstoneData (data, filePath, head, typeData)
	putHead(data, filePath)
	local file = io.open(filePath, "a+")
	for _, v in pairs(data) do
		if v[head] == typeData then
			local pos = 1
			while 1 do 
				file:write(v[data[1][pos]])
				pos = pos + 1
				if data[1][pos] == nil then
					file:write('\n')
					break
				end
				file:write(',')
			end
		end
	end
	file:close()
end

--修改属性 
--1. 数据表   2. 输出位置 3.表头  4. 要修改的属性 5.修改后的属性
local function modifyData (data, filePath, head, BeforAttribute, AfterAttribute)
	putHead(data, filePath)
	local file = io.open(filePath, "a+")
	for _, v in pairs(data) do 
		if v[head] == BeforAttribute then
			v[head] = AfterAttribute
		end
		local pos = 1
		while 1 do 
			if v[head] == nil then
				break
			end
			file:write(v[data[1][pos]])
			pos = pos + 1
			if data[1][pos] == nil then
				file:write("\n")
				break
			end
			file:write(',')
		end
	end
	file:close()
end

--全局替换数据
local function replaceData (data, filePath, find, replace)
	putHead(data, filePath)
    local file = io.open(filePath, "a+")
	for _, v in pairs(data) do 
		local pos = 1
		while 1 do
			if v[data[1][pos]] == nil then
				break
			end
			v[data[1][pos]] = string.gsub(v[data[1][pos]], find, replace) 
			file:write(v[data[1][pos]])
			pos = pos + 1
			if data[1][pos] == nil then
				file:write('\n')
				break
			end
			file:write(',')
		end
	end
    file:close()
end

--打卡文件读取 .csv文件
local function openFile (filePath)
	local file = io.open(filePath, "r")
	if file == nil then 
		print("open error")
		return
	end
	local data = {}
	for line in file:lines() do
		if data[1] == nil then
			local head = {}
			for Temp in string.gmatch(line, '([^,]+)') do
				table.insert(head, Temp)
			end
			table.insert(data, head)
		
		else
			local dataTemp = {}
			local pos = 1
			while 1 do
				local len = string.len(line)
				-- FIXME: 逗号为什么要转义一下？
				nid = string.find(line, ",")
				if nid == nil then
					nid = 0
				end
				dataTemp[data[1][pos]] = string.sub(line, 1, nid -1)
				if nid == 0 then
					table.insert(data, dataTemp)
					break
				end
				nid = nid + 1
				line = string.sub(line, nid, len)
				pos = pos + 1
			end
		end
	end
	file:close()
	return data
end


--解析文件
local materialInformation = openFile("D:\\data.csv")

--输出宝石类
gemstoneData(materialInformation, "D:\\GemstoneData.csv", 'GoodsType', '13')

--修改稀有度
modifyData(materialInformation, "D:\\modifData.csv", 'Rarity', '4', '9')

--把宝石修改成水晶
replaceData(materialInformation, "D:\\replaceData.csv", "宝石", "水晶")



