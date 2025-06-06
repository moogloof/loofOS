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

all_options = {}

with open(f"arch/{arch}/build.json", "r") as f:
	all_options = json.load(f)

# Build the kernel binary
kernel_env = Environment(
	**all_options,
	CPPPATH=[arch_dir, kernel_dir, common_dir]
)

kernel_files = glob.glob(f"arch/{arch}/**/*.[cs]", recursive=True) + glob.glob(f"kernel/**/*.[cs]", recursive=True) + glob.glob(f"common/**/*.[cs]", recursive=True)
kernel_env.Program("kernel", kernel_files)
