import json
import matplotlib.pyplot as plt
import numpy as np

# fichiers_json = [
#     "../datas/sequentiel.json",
#     "../datas/parallele_1.json",
#     "../datas/parallele_2.json",
#     "../datas/parallele_3.json",
#     "../datas/parallele_4.json",
#     "../datas/parallele_5.json",
#     "../datas/parallele_6.json",
#     "../datas/parallele_7.json",
# ]
fichiers_json = ["../datas/parallele_7.json", "../datas/parallele_opti_7.json"]


commands = []
means = []
stddevs = []

# Extract datas from benchmark files
for paths in fichiers_json:
    with open(paths, "r") as f:
        data = json.load(f)
        # Take first result out of it
        res = data["results"][0]

        commands.append(res["command"])
        means.append(res["mean"])
        stddevs.append(res["stddev"])

# Create graphique
plt.figure(figsize=(12, 6))

x_pos = np.arange(len(commands))

bars = plt.bar(
    x_pos,
    means,
    yerr=stddevs,
    capsize=5,
    color="skyblue",
    edgecolor="navy",
    alpha=0.8,
    label="Temps moyen (s)",
)

# Add numeric values
for bar in bars:
    height = bar.get_height()
    plt.text(
        bar.get_x() + bar.get_width() / 2.0,
        height + (max(means) * 0.01),
        f"{height:.4f}s",
        ha="center",
        va="bottom",
        fontsize=9,
    )

# Configure axis
plt.xlabel("Commandes / Configurations", fontsize=12, fontweight="bold")
plt.ylabel("Temps d'exécution moyen [s]", fontsize=12, fontweight="bold")
plt.title(
    "Comparaison des temps d'exécution moyens (Hyperfine)",
    fontsize=14,
    fontweight="bold",
    pad=15,
)

# Remplacement graduation par nom de commande
plt.xticks(x_pos, commands, rotation=15, ha="right")

plt.grid(axis="y", linestyle="--", alpha=0.5)
plt.tight_layout()

# Display or save
plt.savefig("comparaison_benchmarks_before_and_after_opti.png", dpi=300)
plt.show()
