from PIL import Image
import sys

def compare_images(bmp_path, png_path):
    # Open the BMP and PNG images
    bmp_image = Image.open(bmp_path)
    png_image = Image.open(png_path)

    # Convert both images to the same mode (e.g., RGB) for comparison
    bmp_image = bmp_image.convert('RGB')
    png_image = png_image.convert('RGB')

    # Get the size of the images
    bmp_size = bmp_image.size
    png_size = png_image.size

    # Check if the images have the same dimensions
    if bmp_size != png_size:
        print("The images have different dimensions and cannot be compared directly.")
        return 0  # Return 0% similarity if dimensions differ

    # Initialize counters for matching and total pixels
    matching_pixels = 0
    total_pixels = bmp_size[0] * bmp_size[1]

    # Compare the pixel data
    for x in range(bmp_size[0]):
        for y in range(bmp_size[1]):
            bmp_pixel = bmp_image.getpixel((x, y))
            png_pixel = png_image.getpixel((x, y))
            if bmp_pixel == png_pixel:
                matching_pixels += 1

    # Calculate the percentage of similarity
    similarity_percentage = (matching_pixels / total_pixels) * 100
    print(f"The images are {similarity_percentage:.2f}% alike.")
    return similarity_percentage

if __name__ == "__main__":
    # Check if the correct number of arguments is provided
    if len(sys.argv) != 3:
        print("Usage: python compare_images.py <bmp_file> <png_file>")
        sys.exit(1)

    # Get the file paths from command-line arguments
    bmp_file = sys.argv[1]
    png_file = sys.argv[2]

    # Compare the images
    compare_images(bmp_file, png_file)
