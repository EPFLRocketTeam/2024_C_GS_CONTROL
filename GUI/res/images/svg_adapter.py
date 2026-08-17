import lxml.etree as et

FILE_NAME     = "/home/ert/2026_C_GS_SOFTWARE/2024_C_GS_CONTROL/GUI/res/images/prop_firehorn2_V1.svg"
NEW_FILE_NAME = "/home/ert/2026_C_GS_SOFTWARE/2024_C_GS_CONTROL/GUI/res/images/prop_firehorn2_V1.svg"

def prettyprint(element, **kwargs):
  xml = et.tostring(element, pretty_print=True, **kwargs)
  print(xml.decode(), end='')

# Iterative DFS function
def bfs(tree):
  name:str = tree.tag
  if (not name.endswith("svg")):
    style:str = tree.get("style")
    if (style != None):
      atrb = (style.split(";"))

      if len(atrb) > 2:
        value = atrb[0].split(":")[1].strip()
        tree.set("fill",value)
        value = atrb[1].split(":")[1].strip()
        tree.set("stroke",value)
      else: 
        value = atrb[0].split(":")[1].strip()
        tree.set("stroke",value)
      tree.attrib.pop('style', None)

      prettyprint(tree)
      print("\n\n\n")
  for child in tree:
    bfs(child)

xml = et.parse(FILE_NAME)

svg = xml.getroot()

bfs(svg)

xml.write(NEW_FILE_NAME)
