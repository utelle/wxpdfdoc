-- Assign script arguments and check validity
blocksFileName = "Blocks.txt"

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

fo = io.open('unicodeblocks.h','w')

fo:write('// Unicode blocks based on Unicode Standard, Version 18.0.0\n\n')
fo:write('#ifndef UNICODE_BLOCKS_H\n')
fo:write('#define UNICODE_BLOCKS_H\n\n')
fo:write('typedef struct UnicodeBlock_\n')
fo:write('{\n')
fo:write('  wxUint32    start;\n')
fo:write('  wxUint32    end;\n')
fo:write('  const char* name;\n')
fo:write('} UnicodeBlock;\n\n')
fo:write('static const UnicodeBlock unicodeBlocks[] =\n')
fo:write('{\n')

count = 0
for line in io.lines(blocksFileName) do
  rem = string.sub(line,1,1)
  if rem ~= "#" and #line > 0 then
    values = split(line, ";")
    blockranges = values[1]
    blockranges = blockranges:gsub("%.", ";")
    blockranges = blockranges:gsub(";;", ";")
    blockinfo = split(blockranges, ";")
    blockstart = blockinfo[1]
    blockend = blockinfo[2]
    blockname = string.sub(values[2],2)
    fo:write('  { 0x' .. blockstart .. ', 0x' .. blockend .. ', "' .. blockname .. '" },\n')
    count = count + 1
  end
end
print("Number of blocks=" .. count)

fo:write('  {0, 0, NULL}\n')
fo:write('};\n')
fo:write('#endif\n')
fo:close()
