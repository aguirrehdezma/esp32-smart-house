from django.urls import path, include
from rest_framework import routers
from .views import SensorDataViewSet, LastSensorDataView


route = routers.SimpleRouter()
route.register(r'sensor_data', SensorDataViewSet)

urlpatterns = [
    path('api/', include(route.urls)),
    path('', LastSensorDataView.as_view(), name='sensor_data_template'),
]
