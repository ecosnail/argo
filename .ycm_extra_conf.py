import os

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

def get_cxx_include_dir():
    parent_dir = "/usr/include/c++"
    subdirs = os.listdir(parent_dir)
    if len(subdirs) > 0:
        return os.path.join(parent_dir, subdirs[0])
    else:
        return None


def Settings(**kwargs):
    flags = [
        "-Wall",
        "-Wextra",
        "-x", "c++",
        "-std=c++17",

        "-I", os.path.join(SCRIPT_DIR, "include"),
    ]

    cxx_include_dir = get_cxx_include_dir()
    if cxx_include_dir is not None:
        flags += ["-I", cxx_include_dir]

    return {"flags": flags}
