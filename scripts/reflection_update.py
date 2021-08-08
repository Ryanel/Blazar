import glob

# Needed functions
class ReflectedType:
    'Reflected type to emit'
    def __init__(self, name) -> None:
        self.name = name
        self.description=""
        self.functions = []
        self.members = []

def strip_single_argument(data, strip_quotes=True):
    start_of_params = data.find('(') + 1
    end_of_params = data.find(')')
    ret = data[start_of_params:end_of_params]
    if strip_quotes:
        ret = ret.replace("\"", "");
        ret = ret.replace("\'", "");
    return ret

def strip_two_arguments(data, strip_quotes=True):
    start_of_params = data.find('(') + 1
    end_of_params = data.find(')')
    ret = data[start_of_params:end_of_params]
    if strip_quotes:
        ret = ret.replace("\"", "");
        ret = ret.replace("\'", "");

    comma = ret.find(',')
    arg1  = ret[:comma].strip()
    arg2  = ret[comma + 1:].strip()
    return (arg1, arg2)

reflected_list = []
files_to_include = []
current_reflected = None
current_path = 'src/Blazar/Blazar/Component/'

print("INFO | Analysing source code...")

for filename in glob.glob(current_path + '/*.h'):
    fpath = filename.replace('\\', '/')
    fpath = fpath[11:]
    files_to_include.append(fpath)
    header_file = open(filename, "rt")
    header_lines = header_file.readlines()

    line_count = 0
    # Strips the newline character
    for line in header_lines:
        line_count += 1
        data = line.strip()

        # Ignore empty lines
        if not data: continue
        # Ignore any line that does not start with "REFLECTION_"
        if not data.startswith('REFLECTION_'): continue
        
        # Now, check which command this is...
        if data.startswith('REFLECTION_REFLECT'):
            current_reflected = ReflectedType(strip_single_argument(data))
        elif data.startswith('REFLECTION_DESCRIPTION'):
            current_reflected.description = strip_single_argument(data)
        elif data.startswith('REFLECTION_METHOD'):
            current_reflected.functions.append(strip_single_argument(data))
        elif data.startswith('REFLECTION_MEMBER'):
            current_reflected.members.append(strip_single_argument(data))
        elif data.startswith('REFLECTION_END'):
            reflected_list.append(current_reflected)
            current_reflected = None
        else:
            print("> Unknown property:" + data + ", ignoring")
            print("> {:2d}: {}".format(line_count, line.strip()))

# Now, we emit all
print("INFO | " + str(len(reflected_list)) + " types to emit")

source_to_emit = "#include \"bzpch.h\"\n"
source_to_emit += "#include \"entt/entt.hpp\"\n"
source_to_emit += "\n//Component includes\n"
for x in files_to_include: 
    source_to_emit += "#include \""+ x +"\"\n"

source_to_emit += "\n"

source_to_emit += "template<typename Type>\n"
source_to_emit += "Type & get_concrete(entt::registry &registry, entt::entity entity) { return registry.template get<Type>(entity); }\n"

source_to_emit += "\n"

source_to_emit += "// NOTE: Autogenerated by the reflection system. Will be overridden on build.\n"
source_to_emit += "void init_reflected_types() {\n"
source_to_emit += "using namespace entt;\n"
source_to_emit += "using namespace Blazar;\n"
source_to_emit += "using namespace Blazar::Components;\n"

for x in reflected_list:
    source_to_emit += "entt::meta<" + x.name + ">().type(\"" + x.name + "\"_hs)\n"
    source_to_emit += ".prop(\"friendly_name\"_hs, \"" + x.name + "\")\n"
    if x.description != "":
        source_to_emit += ".prop(\"description\"_hs, \"" + x.description + "\")\n"
    
    # Getter function
    source_to_emit += ".func<&get_concrete<"+x.name+">, entt::as_ref_t>(\"get\"_hs)\n"

    for fn in x.functions:
        source_to_emit += ".func<&" + x.name + "::" + fn + ">(\"" + fn + "\"_hs)\n"
    for mem in x.members:
        source_to_emit += ".data<&" + x.name + "::" + mem + ">(\"" + mem + "\"_hs)\n"

    source_to_emit += ";\n"

source_to_emit += "}"

f = open("./build/reflection_types.cpp", "w")
f.write(source_to_emit)
f.close()