import glob
import json
import os

possible_arch = os.listdir("src/arch")
arch = ARGUMENTS.get("ARCH")

if arch is None:
	print("You need to specify the ARCH")
	Exit(1)

if arch not in possible_arch:
	print(f"Only the following architectures are supported: {", ".join(possible_arch)}")
	Exit(1)

arch_dir = f"src/arch/{arch}"
kernel_dir = "src/kernel"
common_dir = "src/common"
system_dir = "src/system"
boot_dir = f"src/boot/{arch}"

all_options = {}

with open(f"{arch_dir}/build.json", "r") as f:
	all_options = json.load(f)

# Build the kernel binary
kernel_env = Environment(
	**all_options,
	CPPPATH=[arch_dir, kernel_dir, common_dir]
)
kernel_files = glob.glob(f"{arch_dir}/**/*.[cs]", recursive=True) + glob.glob(f"{kernel_dir}/**/*.[cs]", recursive=True) + glob.glob(f"{common_dir}/**/*.[cs]", recursive=True)

# Do objects first to avoid basename conflicts (cs)
kernel_objects = []
for f in kernel_files:
	f_target = f + ".o"
	kernel_env.Object(target=f_target, source=f)
	kernel_objects.append(f_target)

kernel_env.Program("kernel", kernel_objects)
