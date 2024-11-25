from django.views.generic import TemplateView
from rest_framework import viewsets
from rest_framework.decorators import api_view
from rest_framework.response import Response
from .models import SensorData, FanControl
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


@api_view(['GET'])
def get_fan_status(request):
    fan_control = FanControl.objects.last()
    return Response({"fan_status": fan_control.fan_status if fan_control else False})


@api_view(['POST'])
def set_fan_status(request):
    new_status = request.data.get('status', False)
    fan_control = FanControl.objects.create(fan_status=new_status)
    return Response({"status": "success", "fan_status": fan_control.fan_status})
