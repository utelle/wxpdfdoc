-- Assign script arguments and check validity
udataFileName = "UnicodeData.txt"

function split(str, character)
  result = {}

  index = 1
-- (.-),
--  for s in string.gmatch(str, "[^"..character.."]+") do
  for s in string.gmatch(str .. character, "(.-)"..character) do
    result[index] = s
    index = index + 1
  end

  return result
end

fo = io.open('unicoderanges.h','w')

fo:write('// Unicode ranges based on Unicode Standard, Version 18.0.0\n\n')
fo:write('#ifndef UNICODE_RANGES_H\n')
fo:write('#define UNICODE_RANGES_H\n\n')
fo:write('typedef struct UnicodeRange_\n')
fo:write('{\n')
fo:write('  wxUint32 first;\n')
fo:write('  wxUint32 last;\n')
fo:write('} UnicodeRange;\n\n')
fo:write('static const UnicodeRange gs_unicodeRanges[] =\n')
fo:write('{\n')

hasFirst = false
prev = -2
count = 0
for line in io.lines(udataFileName) do
  rem = string.sub(line,1,1)
  if rem ~= "#" and #line > 0 then
    values = split(line, ";")
    cid = values[1]
    cidnum = tonumber(cid, 16)
    if hasFirst then
      hasFirst = false
      remark = string.sub(values[2],2,#values[2]-7)
      fo:write('  { 0x' .. rangestart .. ', 0x' .. cid .. ' }, // ' .. remark .. '\n')
      count = count + 1
      rangestart = cid
      prev = -2
    else
      sep1, sep2 = string.find(values[2], "First>")
      if sep1 ~= nil then
        hasFirst = true
        if prev > 0 then
          fo:write('  { 0x' .. rangestart .. ', 0x' .. cidprev .. ' },\n')
          count = count + 1
        end
        rangestart = cid
      else
        if cidnum > prev+1 then
          if prev > 0 then
            fo:write('  { 0x' .. rangestart .. ', 0x' .. cidprev .. ' },\n')
            count = count + 1
          end
          rangestart = cid
        end
      end
      prev = cidnum
    end
    cidprev = cid
  end
end
print("Number of ranges=" .. count)

fo:write('  { 0xffffffff, 0xffffffff } // Sentinel\n')
fo:write('};\n\n')
fo:write('static int gs_unicodeRangeCount = sizeof(gs_unicodeRanges) / sizeof(UnicodeRange);\n\n')
fo:write('#endif\n')
fo:close()
