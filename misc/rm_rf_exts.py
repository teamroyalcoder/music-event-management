import os

def delete_files(directory, extensions):
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            for extension in extensions:
                if file.endswith(extension.strip()):
                    print(f"Deleting file: {file_path}")
                    os.remove(file_path)
                    break

root_directory = input("Enter root directory [./]: ") or './'
file_extensions = input("Enter file extensions (comma-separated) [.save]: ") or '.save'
extensions = [extension.strip() for extension in file_extensions.split(',')]

print(f"\nPerforming recursive file deletion in '{root_directory}' with file extensions: {extensions}\n")

delete_files(root_directory, extensions)
