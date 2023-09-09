class ClassNode:
    def __init__(self, name):
        self.name = name
        self.children = []

    def add_child(self, child):
        self.children.append(child)

    def __str__(self, level=0):
        indent = "    " * level
        result = f"{indent}{self.name}"
        for child in self.children:
            result += f"\n{child.__str__(level + 1)}"
        return result

class ClassHierarchy:
    def __init__(self):
        self.classes = {}

    def add_class(self, name, inherits_from):
        if name not in self.classes:
            self.classes[name] = ClassNode(name)
        if inherits_from:
            parent = self.classes.get(inherits_from, ClassNode(inherits_from))
            parent.add_child(self.classes[name])

    def __str__(self):
        root_nodes = [node for node in self.classes.values() if not any(cls.name == node.name for cls in self.classes.values() if node != cls)]
        result = ""
        for node in root_nodes:
            result += node.__str__() + "\n"
        return result

# Read the input text file
with open("inheritence.txt", "r") as file:
    lines = file.readlines()

hierarchy = ClassHierarchy()

# Process the lines to build the class hierarchy
in_classes_section = False
for line in lines:
    line = line.strip()
    if line == "START CLASSES START CLASSES":
        in_classes_section = True
    elif line == "END CLASSES END CLASSES":
        in_classes_section = False
    if " : " in line:
        class_name, inherits_from = line.split(" : ")
        hierarchy.add_class(class_name.strip(), inherits_from.strip())

# Print the class hierarchy
output = str(hierarchy)
print(output)

# Optionally, you can save the output to a file
with open("output2.txt", "w") as output_file:
    output_file.write(output)
