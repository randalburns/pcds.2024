import ray
from PIL import Image
from torchvision.models import resnet50, ResNet50_Weights
from torchvision.transforms.functional import pil_to_tensor

@ray.remote
class RRN50Normalize:
    """Classify images using the ResNet-50 model from torchvision.
       This is a serial implementation that classifies files one at a time.

    Attributes:
        weights: The weights configuration for the ResNet-50 model.
        preprocess: The transform used to preprocess images for the model.
        model: The ResNet-50 model.
        cats: The category metadata for the model predictions.

    Methods:
        __init__: setup model for classification
        classify_image: Classifies an image using the loaded ResNet-50 model.
    """

    def __init__(self):
        # Load the model weights
        self.weights = ResNet50_Weights.DEFAULT
        # extract the transform to normalize images
        self.preprocess = self.weights.transforms()
        preprocess_img = ResNet50_Weights.DEFAULT.transforms()

    def normalize_image(self, file_path):
        """
        Normalizes an image prior to classification.

        Args:
            file_path: The path to the image file to be normalized.

        Returns:
            TODO
        Raises:
            Exception: If there is an image format error.
        """
        # open image and convert to tensor
        image = Image.open(file_path)
        imgtensor = pil_to_tensor(image)

        try:
            return self.preprocess(imgtensor).unsqueeze(dim=0)
        except:
            raise
