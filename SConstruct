import glob
import json
import os

possible_arch = os.listdir("arch")
arch = ARGUMENTS.get("ARCH")

if arch is None:
	print("You need to specify the ARCH")
	Exit(1)

if arch not in possible_arch:
	print(f"Only the following architectures are supported: {", ".join(possible_arch)}")
	Exit(1)

arch_dir = f"arch/{arch}"
kernel_dir = "kernel"
common_dir = "common"
system_dir = "system"
boot_dir = f"boot/{arch}"
build_dir = "build"

all_options = {}

with open(f"{arch_dir}/build.json", "r") as f:
	all_options = json.load(f)

# Setup environments
# Setup kernel environment
kernel_env = Environment(
	**all_options,
	CPPPATH=[arch_dir, kernel_dir, common_dir]
)
kernel_env.AppendENVPath("PATH", ["/usr/local/bin", "/opt/homebrew/bin"])

# Find kernel source files
kernel_files = glob.glob(f"{arch_dir}/**/*.[cs]", recursive=True) + glob.glob(f"{kernel_dir}/**/*.[cs]", recursive=True) + glob.glob(f"{common_dir}/**/*.[cs]", recursive=True)

try:
	os.mkdir(build_dir)
except FileExistsError:
	pass

# Do objects first to avoid basename conflicts (cs)
kernel_objects = []
for f in kernel_files:
	f_target = f"build/{f}.o"
	kernel_env.Object(target=f_target, source=f)
	kernel_objects.append(f_target)

# Build kernel binary
kernel_builder = Builder(
	action=f"$LD $LINKFLAGS -o $TARGET $SOURCES"
)
kernel_env.Append(BUILDERS={"Kernel": kernel_builder})
kernel_env.Kernel(f"build/kernel.o", kernel_objects)
