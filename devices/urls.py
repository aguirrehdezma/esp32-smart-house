from django.urls import path, include
from rest_framework import routers
from .views import SensorDataViewSet, LastSensorDataView, get_fan_status, set_fan_status


route = routers.SimpleRouter()
route.register(r'sensor_data', SensorDataViewSet)

urlpatterns = [
    path('api/', include(route.urls)),
    path('api/get_fan_status/', get_fan_status, name='get_fan_status'),
    path('api/set_fan_status/', set_fan_status, name='set_fan_status'),
    path('', LastSensorDataView.as_view(), name='sensor_data_template'),
]
