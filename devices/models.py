from django.db import models


class SensorData(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    temperature = models.FloatField()
    motion_detected = models.BooleanField()

    def __str__(self):
        return f"Temp: {self.temperature}, Motion: {self.motion_detected} at {self.timestamp}"
