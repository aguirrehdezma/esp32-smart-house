from django.views.generic import TemplateView
from rest_framework import viewsets
from .models import SensorData
from .serializers import SensorDataSerializer


class SensorDataViewSet(viewsets.ModelViewSet):
    queryset = SensorData.objects.all()
    serializer_class = SensorDataSerializer


class LastSensorDataView(TemplateView):
    template_name = "devices.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        last_sensor_data = SensorData.objects.last()
        context['last_sensor_data'] = last_sensor_data
        return context
